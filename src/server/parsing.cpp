/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/09 18:14:34 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "sstream"

void	Server::_parse_kick(const int &fd, std::string const &line)
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

	try {
		Channel::getChannel(channelName, _clients[fd]).kick(nickname, _clients[fd]);
		_clients[fd].addToSendBuffer(":FT_IRC KICK " + channelName + " " + nickname + " :" + reason + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		_clients[fd].addToSendBuffer(e.what());
		std::cerr << "No such nick/channel/bad privileges" << std::endl;
	}
}