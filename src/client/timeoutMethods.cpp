/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeoutMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:45:38 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/29 17:36:16 by JFikents         ###   ########.fr       */
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

void Client::resetPingTimer(const string &line)
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
	setProgrammedDisconnection(std::chrono::seconds(TIMEOUT));
	_isPingSent = false;
}

void Client::setProgrammedDisconnection(std::chrono::seconds seconds)
{
	_programmedDisconnection = std::chrono::system_clock::now() + seconds;
}
