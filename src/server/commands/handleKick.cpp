/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleKick.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:34:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/15 13:41:13 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

static void	parseLine(const string &line, vector<string> &channels,
	vector<string> &nicks, string &reason, const Client &client)
{
	stringstream	ss(line);
	string			command;
	string			rawChannels;
	string			rawNicks;

	ss >> command >> rawChannels >> rawNicks;
	std::getline(ss >> std::ws, reason);
	if (reason[0] == ':')
		reason = reason.substr(1);
	if (rawChannels.empty() || rawNicks.empty())
		throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getNickname(), command));
	channels = split(rawChannels, ',');
	nicks = split(rawNicks, ',');
}

void Server::_handleKick(Client &client, const string &line)
{
	vector<string>	channels;
	vector<string>	nicks;
	string			reason;

	try {
		parseLine(line, channels, nicks, reason, client);
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
		return ;
	}
	for (string &channel : channels)
	{
		for (const string &nick : nicks)
		{
			try {
				Channel::getChannel(channel).kick(nick, client, reason);
			}
			catch (const std::invalid_argument &e) {
				_logError(client, e.what());
			}
		}
	}
}
