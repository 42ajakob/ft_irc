/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeoutMethods.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:45:38 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/21 20:53:42 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

const t_TimeStamp	&Client::getConnectionTime() const
{
	return (_connectionTime);
}

void Client::setProgrammedDisconnection(std::chrono::seconds seconds)
{
	_programmedDisconnection = std::chrono::system_clock::now() + seconds;
}

const t_TimeStamp	&Client::getProgrammedDisconnection() const
{
	return (_programmedDisconnection);
}

void Client::pingClient()
{
	if (_isPingSent == true)
		return ;
	std::string ping = "PING :" + _Nickname + "\r\n";
	addToSendBuffer(ping);
	_isPingSent = true;
}

void Client::resetPingTimer()
{
	if (this->IsRegistered() == true)
	{
		_programmedDisconnection = std::chrono::system_clock::now() + std::chrono::seconds(60);
		_isPingSent = false;
	}
}
