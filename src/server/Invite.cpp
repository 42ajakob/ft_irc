/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:38:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/09 18:02:56 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>


void Server::_invite(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				nickname;
	string				channelName;

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
		client.addToSendBuffer(ERR_NOSUCHCHANNEL(channelName));
		std::cerr << "Error inviting user: " << e.what() << std::endl;
	}
}
