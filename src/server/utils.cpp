/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 20:27:36 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/28 17:53:07 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static size_t	jumpSpaces(const std::string &line, size_t pos)
{
	auto it = line.begin() + pos;
	auto end = line.end();

	while (it != end && *it == ' ')
		++it;
	return (it - line.begin());
}

size_t	findNextParameter(const std::string &line, size_t pos)
{
	pos = jumpSpaces(line, pos);
	size_t next_space = line.find(' ', pos);
	size_t next_colon = line.find(':', pos);

	if (line[pos] == ':'
		|| (next_space == std::string::npos && next_colon == std::string::npos))
		return (std::string::npos);
	if (next_space == std::string::npos)
		return (next_colon);
	next_space = jumpSpaces(line, next_space);
	if (next_colon == std::string::npos || next_space < next_colon)
		return (next_space);
	return (next_colon);
}
