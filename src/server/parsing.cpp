/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/13 16:25:35 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

void	Server::_handleKick(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				channelName;
	string				nick;
	string				reason;

	ss >> channelName >> channelName >> nick >> reason;

	toLower(nick);

	if (channelName.empty() || nick.empty())
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));

	try {
		Channel::getChannel(channelName).kick(nick, client);
		client.addToSendBuffer(":FT_IRC KICK " + channelName + " " + nick + " :" + reason + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}

void	Server::_handleMode(Client &client, string const &line)
{
	std::stringstream	ss(line);
	string			command;
	string			channelName;
	string			mode;
	string			nick;

	ss >> command >> channelName >> mode >> nick;

	if ((mode == "+o" || mode == "-o") && nick.empty())
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));

	try {
		if (!nick.empty())
			Channel::getChannel(channelName).mode(mode, client, nick);
		else
			Channel::getChannel(channelName).mode(mode, client);

		client.addToSendBuffer(":FT_IRC MODE " + channelName + " " + mode + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}
