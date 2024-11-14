/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/14 19:06:16 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

bool isCharInSet(char c) {
    return string("okl").find(c) != string::npos;
}


void	Server::_modeLoop(Client &client, Channel &channel, const string &mode, string &mode_params)
{
	string	plus_minus;
	string	mode_parameter;
	size_t	i;
	std::stringstream	ss(mode_params);

	plus_minus = mode.substr(0, 1);
	i = 1;

	while (i < mode.size())
	{
		try {
			if (isCharInSet(mode[i]))
			{
				ss >> mode_parameter;	
				channel.mode(plus_minus + mode[i], client, mode_parameter);
			}
			else
				channel.mode(plus_minus + mode[i], client);
		}
		catch (const std::invalid_argument &e) {
			_logError(client, e.what());
		}
		i++;
	}
	std::getline(ss >> std::ws, mode_params);
}

void	Server::_handleMode(Client &client, string const &line)
{
	stringstream	ss(line);
	Channel			*channel;
	string			command;
	string			channelName;
	string			mode;
	string			mode_params;
	string			setMode;
	string			unsetMode;

	ss >> command >> channelName >> mode;
	std::getline(ss >> std::ws, mode_params);

	if (channelName.empty())
	{
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), command));
		return ;
	}
	try {
		channel = &Channel::getChannel(channelName);

		if (mode.empty())
		{
			channel->mode(mode, client);
		}
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
		return ;
	}

	if (mode[0] == '+')
	{
		setMode = mode.substr(0, mode.find('-'));
		unsetMode = mode.substr(mode.find('-'));
	}
	else if (mode[0] == '-')
	{
		setMode = mode.substr(mode.find('+'));
		unsetMode = mode.substr(0, mode.find('+'));
	}
	_modeLoop(client, *channel, setMode, mode_params);
	_modeLoop(client, *channel, unsetMode, mode_params);
}
