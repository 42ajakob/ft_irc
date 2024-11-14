/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleKick.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:34:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/14 18:12:29 by JFikents         ###   ########.fr       */
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
	string			rawChannels;
	string			rawNicks;
	string			command;

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
	string			reason;
	vector<string>	channels;
	vector<string>	nicks;

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
