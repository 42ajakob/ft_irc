/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:09:21 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/05 14:22:28 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include <stdexcept>
#include <vector>

std::vector<string>	split(const string &line, const char &delimiter)
{
	std::vector<string>	result;
	string				workingLine = line;
	size_t				pos = workingLine.find(delimiter);

	while (pos != std::string::npos)
	{
		result.emplace_back(workingLine.substr(0, pos));
		workingLine.erase(0, pos + 1);
		pos = workingLine.find(delimiter);
	}
	result.emplace_back(std::move(workingLine));
	return (result);
}

void	splitPasswordsAndChannels(const string &line,
	std::vector<string> &channelNames,
	std::vector<string> &passwords)
{
	size_t	pos = findNextParameter(line);
	string	rawChannelNames;

	if (pos == string::npos)
		throw std::runtime_error("JOIN command without channel name");
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
		// _clients[fd]->addToSendBuffer("451 You have not registered\n");
		return;
	}
	splitPasswordsAndChannels(line, channelNames, passwords);
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		string	ChannelName = channelNames[i];
		string	Password = passwords[i];
		toLower(ChannelName);
		Channel	&channel = Channel::getChannel(ChannelName, _clients[fd]);

		channel.join(_clients[fd], Password);
		std::cout << "Client " << _clients[fd].getNickname() << " joined channel " << ChannelName << std::endl;
		std::cout << "Password: <" << Password + '>' << std::endl;
		channel.printMembers();
	}
}
