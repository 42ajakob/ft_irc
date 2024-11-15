/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:10 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/13 14:12:35 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <iostream>
# include <chrono>
# include <unordered_set>
# include <string>
# include <vector>

# define TIMEOUT 300

using std::string;
using std::vector;
using std::stringstream;

typedef std::chrono::time_point<std::chrono::system_clock> t_TimeStamp;
typedef std::unordered_set<string>	t_StringSet;

constexpr size_t MAX_STREAM_SIZE = std::numeric_limits<std::streamsize>::max();

enum class eCommand
{
	PING,
	PONG,
	CAP,
	PASS,
	NICK,
	USER,
	QUIT,
	PRIVMSG,
	JOIN,
	OPER,
	RM_OPER,
	ADD_OPER,
	LS_OPER,
	RELOAD_SERVER,
	TOPIC,
	KICK,
	INVITE,
	MODE,
	PART,
	WHO,
	BYPASS,
	UNKNOWN
};

size_t			findNextParameter(const string &line, size_t pos = 0);
void			toLower(string &str);
vector<string>	split(const string &string, const char &delim = ',');

#endif