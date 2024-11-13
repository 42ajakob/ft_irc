/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleJoin.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:09:21 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 17:20:47 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <vector>
#include <sstream>

void	splitPasswordsAndChannels(const string &line,
	vector<string> &channelNames, vector<string> &passwords,
	const string &clientNickname)
{
	stringstream	ss(line);
	string			command;
	string			rawChannelNames;
	string			rawPasswords;

	ss >> command >> rawChannelNames >> rawPasswords;
	if (rawChannelNames.empty())
		throw std::invalid_argument(ERR_NEEDMOREPARAMS(clientNickname, command));
	if (rawPasswords.empty() == false)
		passwords = split(rawPasswords);
	channelNames = split(rawChannelNames);
	while (channelNames.size() > passwords.size())
		passwords.emplace_back("");
}

static void	logJoiningChannel(const string &clientNickname,
	const string &ChannelName, const string &Password, Channel &channel)
{
	std::cout << "Client " << clientNickname << " joined channel " << ChannelName << std::endl;
	std::cout << "Password: <" << Password + '>' << std::endl;
	channel.printMembers();
}

void	Server::_handleJoin(Client	&client, const string &line)
{
	vector<string>	channelNames;
	vector<string>	passwords;
	string			clientNickname = client.getNickname();

	try {
		if (client.IsRegistered() == false)
			throw std::runtime_error(ERR_NOTREGISTERED(clientNickname));
		splitPasswordsAndChannels(line, channelNames, passwords, clientNickname);
	}
	catch (std::runtime_error &e) {
		return (_logError(client, e.what()));
	}
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		try {
			Channel	&channel = Channel::getChannel(channelNames[i], client);

			channel.join(client, passwords[i]);
			logJoiningChannel(clientNickname, channelNames[i], passwords[i], channel);
		}
		catch (const std::exception &e) {
			_logError(client, e.what());
		}
	}
}
