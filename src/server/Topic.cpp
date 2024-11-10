/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:29:00 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 15:53:03 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::_topic(const int &fd, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	topic;

	if (pos == string::npos)
		return ;
	if (line[pos] == ':')
		pos++;
	topic = line.substr(pos);
	try {
		Channel	&channel = Channel::getChannel(channelName);
		channel.topic(topic, client);
	}
	catch (const std::exception &e) {
		client.addToSendBuffer(ERR_(channelName));
		std::cerr << "Error setting topic: " << e.what() << std::endl;
	}
}
