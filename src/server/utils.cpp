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

static std::string::iterator	jumpSpaces(std::string::iterator it, std::string::iterator end)
{
	while (it != end && *it == ' ')
		it++;
	return (it);
}

size_t	Server::findNextParameter(std::string &line, size_t pos = 0)
{
	auto it = line.begin() + pos;
	auto end = line.end();

	it = jumpSpaces(it, end);
	if (it == end)
		return (std::string::npos);
	auto next_space = line.find(' ', pos);
	auto next_colon = line.find(':', pos);

	if (next_space == std::string::npos && next_colon == std::string::npos)
		return (std::string::npos);
	if (next_space == std::string::npos)
		return (next_colon);
	if (next_colon == std::string::npos || next_space < next_colon)
		return (next_space);
	return (next_colon);
}

// size_t	Server::findNextParameter(std::string &line)
// {
// 	auto next_space = line.find(' ');
// 	auto next_colon = line.find(':');
// 	auto end = line.end();

// 	if (next_space == end && next_colon == end)
// 		return (end);
// 	if (next_space == end)
// 		return (next_colon);
// 	if (next_colon == end || next_space < next_colon)
// 		return (next_space);
// 	return (next_colon);
// }

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