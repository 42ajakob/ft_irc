/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:27:36 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/07 15:32:15 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>

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
	size_t next_space = line.find(' ', pos);
	size_t next_colon = line.find(':', pos);

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
	auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "on %d.%m.%Y at %H:%M:%S");

    _timestamp =  oss.str();
}

std::string Server::getTimestamp()
{
	return (_timestamp);	
}