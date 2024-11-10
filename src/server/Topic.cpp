/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 15:29:00 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 15:31:08 by apeposhi         ###   ########.fr       */
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
	_clients[fd].getChannel().setTopic(topic);
	_clients[fd].getChannel().sendChannelInfo(_clients[fd]);
}
