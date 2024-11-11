/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/11 16:41:56 by JFikents         ###   ########.fr       */
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

	try {
		Channel::getChannel(channelName, client).kick(nickname, client);
		client.addToSendBuffer(":FT_IRC KICK " + channelName + " " + nickname + " :" + reason + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		client.addToSendBuffer(e.what());
		std::cerr << "No such nick/channel/bad privileges" << std::endl;
	}
}