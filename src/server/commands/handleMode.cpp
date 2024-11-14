/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/14 20:56:04 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

bool isCharInSet(char c) {
	return (string("okl").find(c) != string::npos);
}


void	Server::_modeLoop(Client &client, Channel &channel, const string &modes, string &mode_params)
{
	const string		plus_minus(modes.substr(0, 1));
	string				mode_parameter;
	size_t				i;
	std::stringstream	ss(mode_params);

	i = 1;

	while (i < modes.size())
	{
		try {
			if (isCharInSet(modes[i]))
			{
				ss >> mode_parameter;
				channel.mode(plus_minus + modes[i], client, mode_parameter);
			}
			else
				channel.mode(plus_minus + modes[i], client);
		}
		catch (const std::invalid_argument &e) {
			_logError(client, e.what());
		}
		i++;
	}
	std::getline(ss >> std::ws, mode_params);
}

static void	parseLine(const string &line, Channel **channel, string &modes,
	string &mode_params, Client &client)
{
	stringstream	ss(line);
	string			command;
	string			channelName;

	ss >> command >> channelName >> modes;
	std::getline(ss >> std::ws, mode_params);
	if (channelName.empty())
		throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getNickname(), command));
	*channel = &Channel::getChannel(channelName);
	if (modes == "b")
		client.addToSendBuffer(RPL_ENDOFBANLIST(channelName));
}

void	Server::_handleMode(Client &client, string const &line)
{
	Channel	*channel;
	string	modes;
	string	mode_params;
	string	setMode;
	string	unsetMode;

	try {
		parseLine(line, &channel, modes, mode_params, client);
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
		return ;
	}
	if (modes.empty())
		channel->sendModes(client);
	if (modes == "b" || modes.empty())
		return ;
	if (modes[0] == '+')
	{
		setMode = modes.substr(0, modes.find('-'));
		if (modes.find('-') != string::npos)
			unsetMode = modes.substr(modes.find('-'));
	}
	else if (modes[0] == '-')
	{
		if (modes.find('+') != string::npos)
		{
			setMode = modes.substr(modes.find('+'));
			unsetMode = modes.substr(0, modes.find('+'));
		}
		else
			unsetMode = modes;
	}
	_modeLoop(client, *channel, setMode, mode_params);
	_modeLoop(client, *channel, unsetMode, mode_params);
}
