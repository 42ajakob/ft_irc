/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 17:46:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/10 19:54:55 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>

static void	parsePrivMsg(const string &line, vector<string> &targets,
	string &msg)
{
	std::stringstream	ss(line);
	string				rawTargets;

	ss.ignore(MAX_STREAM_SIZE, ' ');
	ss >> rawTargets;
	if (rawTargets.empty() || rawTargets[0] == ':')
		throw std::invalid_argument(ERR_NORECIPIENT(string("PRIVMSG")));
	targets = split(rawTargets, ',');
	if (targets.size() > 4)
		throw std::invalid_argument(ERR_TOOMANYTARGETS(rawTargets));
	msg = line.substr(ss.tellg());
	if (!msg.empty() && msg[0] == ':')
		msg.erase(0, 1);
	if (msg.empty())
		throw std::invalid_argument(ERR_NOTEXTTOSEND());
}

static	void	sendMsgToTarget(const string &origin, const string &msg,
	const string &target)
{
	if (target[0] == '#')
	{
		Channel &channel = Channel::getChannel(target);
		channel.broadcastMsg(msg, origin);
		return ;
	}
	Client &receiver = Server::getInstance().getClientByNickname(target);
	receiver.sendPrivMsg(msg, origin);
}

void	Server::_privmsg(Client &client, const string &line) noexcept
{
	vector<string>		targets;
	string				msg;
	string				origin = client.getNickname();

	try {
		parsePrivMsg(line, targets, msg);
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
		return ;
	}
	for (const string &target : targets)
	{
		try {
			sendMsgToTarget(origin, msg, target);
		}
		catch (const std::invalid_argument &e) {
			_logError(client, e.what());
		}
	}
}
