/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:40:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/10 19:58:51 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void	Client::addToSendBuffer(string buffer) noexcept
{
	_sendBuffer += buffer;
}

void	Client::addToRecvBuffer(string buffer) noexcept
{
	_recvBuffer += buffer;
}

void	Client::clearSendBuffer()
{
	_sendBuffer.clear();
}

void	Client::clearRecvBuffer()
{
	_recvBuffer.clear();
}

const string	&Client::getSendBuffer() const
{
	return (_sendBuffer);
}

const string	&Client::getRecvBuffer() const
{
	return (_recvBuffer);
}

void	Client::sendPrivMsg(const string &msg, const string &origin) noexcept
{
	addToSendBuffer(":" + origin + " PRIVMSG " + _Nickname + " :" + msg + "\r\n");
}
