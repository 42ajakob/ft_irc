/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePart.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 21:52:40 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 17:44:11 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include <sstream>

void	Server::_handlePart(Client &client, const string &line)
{
	stringstream	ss(line);
	string			rawChannelNames;
	string			reason;
	string			command;

	ss >> command >> rawChannelNames;
	if (rawChannelNames.empty())
	{
		_logError(client, ERR_NEEDMOREPARAMS(client.getNickname(), "PART"));
		return ;
	}
	std::getline(ss >> std::ws, reason);
	if (!reason.empty() && reason[0] == ':')
		reason = reason.erase(0, 1);
	for (string &ChannelName : split(rawChannelNames))
	{
		try {
			Channel &channel = Channel::getChannel(ChannelName);
			channel.part(client, reason);
		}
		catch (const std::exception &e) {
			_logError(client, e.what());
		}
	}
}
