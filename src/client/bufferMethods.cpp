/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bufferMethods.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:40:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/09 17:44:42 by apeposhi         ###   ########.fr       */
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
