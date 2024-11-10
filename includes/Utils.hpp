/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:10 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 16:26:03 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <iostream>
# include <chrono>
# include <unordered_set>

# define TIMEOUT 300

using std::string;

typedef std::chrono::time_point<std::chrono::system_clock> t_TimeStamp;
typedef std::unordered_set<string>	t_StringSet;

enum class eCommand
{
	UNKNOWN,
	PING,
	PONG,
	PRIVMSG,
	JOIN,
	NICK,
	USER,
	QUIT,
	PASS,
	CAP,
	OPER,
	RM_OPER,
	ADD_OPER,
	LS_OPER,
	RELOAD_SERVER,
	TOPIC,
	KICK,
	INVITE,
	MODE,
	DEBUG_BYPASS
};

size_t	findNextParameter(const std::string &line, size_t pos = 0);
void	toLower(std::string &str);

#endif