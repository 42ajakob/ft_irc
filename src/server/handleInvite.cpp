/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleInvite.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:38:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/13 14:15:52 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>


void Server::_handleInvite(Client &client, const string &line)
{
	stringstream	ss(line);
	string			nickname;
	string			channelName;

	ss >> nickname;
	ss >> nickname;
	ss >> channelName;
	std::cout << "Inviting " << nickname << " to " << channelName << std::endl;
	if (nickname.empty() || channelName.empty())
		return client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
	if (channelName[0] == ':')
		channelName.erase(0, 1);
	if (channelName[0] != '#')
		return client.addToSendBuffer(ERR_NOSUCHCHANNEL(channelName));
	try {
		Channel	&channel = Channel::getChannel(channelName);
		channel.invite(nickname, client);
	}
	catch (const std::exception &e) {
		client.addToSendBuffer(e.what());
		std::cerr << "Error inviting user: No such Channel" << std::endl;
	}
}