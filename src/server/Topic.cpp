/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:29:00 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 18:02:15 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

void Server::_topic(Client &client, const string &line)
{
	size_t	pos = findNextParameter(line);
	std::stringstream	ss(line);
	string	topic;
	string	channelName;

	ss >> channelName;
	ss >> channelName;
	pos = findNextParameter(line, pos);
	if (pos != string::npos)
		topic = line.substr(pos);
	try {
		Channel	&channel = Channel::getChannel(channelName);
		channel.topic(topic, client);
	}
	catch (const std::exception &e) {
		client.addToSendBuffer(ERR_NOSUCHCHANNEL(channelName));
		std::cerr << "Error setting topic: " << e.what() << std::endl;
	}
}
