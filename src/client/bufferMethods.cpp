/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:40:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/21 17:41:15 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

const std::string	&Client::getSendBuffer() const
{
	return (_sendBuffer);
}

const std::string	&Client::getRecvBuffer() const
{
	return (_recvBuffer);
}

void	Client::addToSendBuffer(std::string buffer)
{
	_sendBuffer += buffer;
}

void	Client::addToRecvBuffer(std::string buffer)
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
