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
	stringstream	ss(line);
	string			channelName;
	string			nickname;
	string			reason;

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
	stringstream	ss(line);
	string			command;
	string			channelName;
	string			mode;
	string			nick;

	ss >> command >> channelName >> mode >> nick;

	if (channelName.empty()
		|| (nick.empty() && (mode == "+o" || mode == "-o")))
	{
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), command));
		return ;
	}
	try {
		Client	&target = this->getClientByNickname(nick);
		Channel	&channel = Channel::getChannel(channelName);
		if (!nick.empty())
			channel.mode(mode, client, target);
		else
			channel.mode(mode, client);
		client.addToSendBuffer(":FT_IRC MODE " + channelName + " " + mode + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}
