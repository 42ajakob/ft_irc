/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:27:36 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/26 20:56:21 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
