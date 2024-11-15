/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleWho.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 22:33:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 20:13:56 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include "Utils.hpp"
#include <sstream>

inline void	_clientWhoReply(Client &client, const string &mask, t_ClientMap &clients)
{
	string	flags = "H";

	for (const auto &[fd, target] : clients)
	{
		if (target.getNickname().find(mask) != string::npos)
		{
			if (target.isOperator())
				flags += "*";
			client.addToSendBuffer(RPL_WHOREPLY(client.getNickname(), "*",
				target.getUsername(), target.getHostname(),
				target.getNickname(), flags));
		}
		flags = "H";
	}
}

inline void	_channelWhoReply(Client &client, string &mask)
{
	Channel	&channel = Channel::getChannel(mask);

	channel.whoReply(client);
}

void	Server::_handleWho(Client &client, const string &line)
{
	stringstream	ss(line);
	string			mask;
	string			command;
	string			flags = "H";

	ss >> command >> mask;
	if (mask.empty())
	{
		_logError(client, ERR_NEEDMOREPARAMS(client.getNickname(), "WHO"));
		return ;
	}
	try {
		if (mask[0] != '#')
			_clientWhoReply(client, mask, _clients);
		else
			_channelWhoReply(client, mask);
		client.addToSendBuffer(RPL_ENDOFWHO(client.getNickname(), mask));
	}
	catch (const std::exception &e) {
		_logError(client, e.what());
	}
}
