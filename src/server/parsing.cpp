/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/10 15:43:35 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "sstream"

void	Server::_parse_kick(Client &client, std::string const &line)
{
	std::stringstream	ss(line);
	std::string			command;
	std::string			channelName;
	std::string			nickname;
	std::string			reason;

	ss >> command >> channelName >> nickname >> reason;
	toLower(channelName);
	toLower(nickname);
	toLower(reason);

	if (channelName.empty() || nickname.empty())
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));

	try {
		Channel::getChannel(channelName).kick(nickname, client);
		client.addToSendBuffer(":FT_IRC KICK " + channelName + " " + nickname + " :" + reason + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		client.addToSendBuffer(e.what());
		std::cerr << "No such nick/channel/bad privileges" << std::endl;
	}
}

void	Server::_parse_mode(Client &client, std::string const &line)
{
	std::stringstream	ss(line);
	std::string			command;
	std::string			channelName;
	std::string			mode;

	ss >> command >> channelName >> mode;

	toLower(channelName);
	toLower(mode);

	if (channelName.empty() || mode.empty())
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));

	try {
		Channel::getChannel(channelName).mode(mode, client);
		client.addToSendBuffer(":FT_IRC MODE " + channelName + " " + mode + "\r\n");
		std::cerr << "MODE " << channelName << " " << mode << std::endl;
	}
	catch (const std::invalid_argument &e) {
		client.addToSendBuffer(e.what());
		std::cerr << "ERROR: Setting mode" << std::endl;
	}
}