/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/17 17:29:09 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

static bool requiresModeParam(string mode)
{
	if (string("okl").find(mode[1]) == string::npos
		|| mode == "-l" || mode == "-k")
		return (false);
	return (true);
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
			if (requiresModeParam(plus_minus + modes[i]))
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

	try {
		parseLine(line, &channel, modes, mode_params, client);
		if (modes.empty())
			channel->sendModes(client);
		if (modes == "b" || modes.empty())
			return ;
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
		return ;
	}

	auto processModes = [&](char secondary) {
		size_t separatorPos = modes.find(secondary);
		string primaryMode = modes.substr(0, separatorPos);
		string secondaryMode = (separatorPos != string::npos) ? modes.substr(separatorPos) : "";

		if (!primaryMode.empty())
			_modeLoop(client, *channel, primaryMode, mode_params);
		if (!secondaryMode.empty())
			_modeLoop(client, *channel, secondaryMode, mode_params);
	};

	if (modes[0] == '+')
		processModes('-');
	else if (modes[0] == '-')
		processModes('+');
}
