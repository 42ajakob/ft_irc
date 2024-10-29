/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinChannel.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:09:21 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/29 17:28:32 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
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

void	Server::joinChannel(const int &fd, string &line)
{
	size_t					pos = findNextParameter(line);
	string					rawChannelNames;
	std::vector<string>		channelNames;
	std::vector<string>		passwords;

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
	for (size_t i = 0; i < channelNames.size(); i++)
	{
		string	ChannelName = channelNames[i];
		string	Password = passwords[i];
		toLower(ChannelName);
		// TODO: Decide on Channel creation method
		std::cout << "Client " << _clients[fd].getNickname() << " joined channel " << ChannelName << std::endl;
		std::cout << "Password: <" << Password + '>' << std::endl;
	}
}
