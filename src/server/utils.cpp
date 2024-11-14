/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:27:36 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/14 15:01:59 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Utils.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <chrono>

Client &Server::getClientByNickname(const string &nickname)
{
	string	searchableNickname(nickname);

	toLower(searchableNickname);
	auto it = std::find_if(_clients.begin(), _clients.end(),
		[&searchableNickname](std::pair<const int, Client> &client)
		{
			return (client.second == searchableNickname);
		});
	if (it != _clients.end())
		return (it->second);
	throw std::invalid_argument(ERR_NOSUCHNICK(nickname));
}

static size_t	jumpSpaces(const string &line, size_t pos)
{
	auto it = line.begin() + pos;
	auto end = line.end();

	while (it != end && *it == ' ')
		++it;
	return (it - line.begin());
}

size_t	findNextParameter(const string &line, size_t pos)
{
	pos = jumpSpaces(line, pos);
	size_t	next_space = line.find(' ', pos);
	size_t	next_colon = line.find(':', pos);

	if (line[pos] == ':'
		|| (next_space == string::npos && next_colon == string::npos))
		return (string::npos);
	if (next_space == string::npos)
		return (next_colon);
	next_space = jumpSpaces(line, next_space);
	if (next_colon == string::npos || next_space < next_colon)
		return (next_space);
	return (next_colon);
}

void	toLower(string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void	Server::setTimestamp()
{
	auto	t = std::time(nullptr);
	auto	tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "on %d.%m.%Y at %H:%M:%S");

	_timestamp = oss.str();
}


string Server::getTimestamp()
{
	return (_timestamp);	
}

vector<string>	split(const string &line, const char &delimiter)
{
	vector<string>	result;
	string			workingLine = line;
	size_t			pos = workingLine.find(delimiter);

	while (pos != string::npos)
	{
		result.emplace_back(workingLine.substr(0, pos));
		workingLine.erase(0, pos + 1);
		pos = workingLine.find(delimiter);
	}
	result.emplace_back(std::move(workingLine));
	return (result);
}

void	Server::_logError(Client &client, const string &error) const
{
	client.addToSendBuffer(error);
	std::cerr << error << std::endl;
}
