/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeoutMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:45:38 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/30 14:45:01 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

const t_TimeStamp	&Client::getProgrammedDisconnection() const
{
	return (_programmedDisconnection);
}

void Client::pingClient()
{
	if (_isPingSent == true)
		return ;
	string ping = "PING :" + _Nickname + "\r\n";
	addToSendBuffer(ping);
	_isPingSent = true;
}

void Client::resetPingTimerIfPongMatches(const string &line)
{
	std::size_t	pos = findNextParameter(line);
	string		pong;

	if (pos == string::npos)
		return ;
	if (line[pos] == ':')
		pos++;
	pong = line.substr(pos);
	if (pong != _Nickname)
		return ;
	setProgrammedDisconnection(TIMEOUT);
	_isPingSent = false;
}

void Client::setProgrammedDisconnection(const int seconds)
{
	_programmedDisconnection = std::chrono::system_clock::now() + std::chrono::seconds(seconds);
}
