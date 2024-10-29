/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:10 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/28 20:27:17 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
# include <chrono>

# define TIMEOUT 90
typedef std::chrono::time_point<std::chrono::system_clock> t_TimeStamp;

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
	DEBUG_BYPASS
};

size_t	findNextParameter(const std::string &line, size_t pos = 0);
void	toLower(std::string &str);

class Utils
{
private:
	/* data */
public:
	Utils();
	~Utils();
	Utils(const Utils &other);
	Utils &operator=(const Utils &other);
	std::string getTimestamp();
};

#endif
