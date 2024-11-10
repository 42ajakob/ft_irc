/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:09:21 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/10 18:21:45 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <vector>

void	splitPasswordsAndChannels(const string &line,
	std::vector<string> &channelNames,
	std::vector<string> &passwords)
{
	size_t	pos = findNextParameter(line);
	string	rawChannelNames;

	if (pos == string::npos)
		throw std::runtime_error("Missing channel name");
	rawChannelNames = line.substr(pos);
	if ((pos = findNextParameter(rawChannelNames)) != string::npos)
	{
		passwords = split(rawChannelNames.substr(pos), ',');
		rawChannelNames.erase(pos - 1);
	}
	channelNames = split(rawChannelNames, ',');
	while (channelNames.size() > passwords.size())
		passwords.emplace_back("");
}

void	Server::_joinChannel(const int &fd, string &line)
{
	std::vector<string>		channelNames;
	std::vector<string>		passwords;

	if (_clients[fd].IsRegistered() == false)
	{
		_clients[fd].addToSendBuffer(ERR_NOTREGISTERED(_clients[fd].getNickname()));
		return;
	}
	try{
		splitPasswordsAndChannels(line, channelNames, passwords);
	}
	catch (std::runtime_error &e)
	{
		_clients[fd].addToSendBuffer(ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), line));
		std::cerr << e.what() << std::endl;
		return ;
	}
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		string	ChannelName = channelNames[i];
		string	Password = passwords[i];
		toLower(ChannelName);
		Channel	&channel = Channel::getChannel(ChannelName, _clients[fd]);

		channel.join(_clients[fd], Password);
		std::cout << "Client " << _clients[fd].getNickname() << " joined channel " << ChannelName << std::endl; // Topic
		std::cout << "Password: <" << Password + '>' << std::endl;
		channel.printMembers(); // Numeric?
	}
}
