/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/12 00:31:14 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

void	Server::_handleKick(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				channelName;
	string				nickname;
	string				reason;

	ss >> channelName >> channelName >> nickname >> reason;

	toLower(channelName);
	toLower(nickname);

	if (channelName.empty() || nickname.empty())
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));

	try {
		Channel::getChannel(channelName).kick(nickname, client);
		client.addToSendBuffer(":FT_IRC KICK " + channelName + " " + nickname + " :" + reason + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}

void	Server::_handleMode(Client &client, string const &line)
{
	std::stringstream	ss(line);
	string			channelName;
	string			mode;
	string			nick;

	ss >> channelName >> channelName >> mode >> nick;

	toLower(channelName);
	toLower(mode);
	toLower(nick);

	if ((channelName.empty() || mode.empty()) || ((mode == "+o" || mode == "-o") && nick.empty()))
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));

	try {
		if (!nick.empty()) {
			Channel::getChannel(channelName).mode(mode, client, this->getClientByNickname(nick));
		}
		else {
			Channel::getChannel(channelName).mode(mode, client);
		}
		client.addToSendBuffer(":FT_IRC MODE " + channelName + " " + mode + "\r\n");
		std::cout << "MODE " << channelName << " " << mode << std::endl;
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}
