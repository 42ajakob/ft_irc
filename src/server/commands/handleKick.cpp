/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleKick.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:34:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/14 16:19:10 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

void Server::_handleKick(Client &client, const string &line)
{
	stringstream ss(line);
	string channel;
	string command;
	string nick;
	string reason;
	int i;
	int j;

	ss >> command >> channel >> nick >> reason;
	vector<string> channels = split(channel, ',');
	vector<string> nicks = split(nick, ',');
	i = 0;

	while (i < channels.size())
	{
		j = 0;
		while (j < nicks.size())
		{
			if (channels.empty() || nicks.empty())
			{
				client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), command));
				return ;
			}
			try
			{
				Channel::getChannel(channels[i]).kick(nicks[j], client, reason);
				/*
				It should be sent to the channel, not the client, and the origin should
					be the client, not the server. I added a "fix" in the Channel::kick()
					method, but it is missing the reason.

				The reason is optional, so it should be checked if it is empty before
					sending it.
				See Channel::part() for an example.
				*/
			}
			catch (const std::invalid_argument &e)
			{
				_logError(client, e.what());
			}
			j++;
		}
		i++;
	}
}
