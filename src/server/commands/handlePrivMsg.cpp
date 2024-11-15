/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlePrivMsg.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 17:46:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 20:39:36 by JFikents         ###   ########.fr       */
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
	stringstream	ss(line);
	string			rawTargets;
	string			command;

	ss >> command >> rawTargets;
	if (rawTargets.empty() || rawTargets[0] == ':')
		throw std::invalid_argument(ERR_NORECIPIENT(command));
	targets = split(rawTargets, ',');
	if (targets.size() > 4)
		throw std::invalid_argument(ERR_TOOMANYTARGETS(rawTargets));
	std::getline(ss >> std::ws, msg);
	if (!msg.empty() && msg[0] == ':')
		msg.erase(0, 1);
	if (msg.empty())
		throw std::invalid_argument(ERR_NOTEXTTOSEND());
}

static	void	sendMsgToTarget(const Client &origin, const string &msg,
	string &target)
{
	if (target[0] == '#')
	{
		Channel &channel = Channel::getChannel(target);
		channel.PrivMsg(msg, origin);
		return ;
	}
	Client &receiver = Server::getInstance().getClientByNickname(target);
	string	originStr = origin.getNickname() + "!" + origin.getUsername()
		+ "@" + origin.getHostname();
	receiver.sendPrivMsg(msg, originStr);
}

void	Server::_handlePrivMsg(Client &client, const string &line) noexcept
{
	vector<string>		targets;
	string				msg;

	try {
		parsePrivMsg(line, targets, msg);
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
		return ;
	}
	for (string &target : targets)
	{
		try {
			sendMsgToTarget(client, msg, target);
		}
		catch (const std::invalid_argument &e) {
			_logError(client, e.what());
		}
	}
}
