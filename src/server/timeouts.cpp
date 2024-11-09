/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeouts.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:19:28 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/09 12:39:45 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <chrono>

void	Server::_checkConnectionTimeout(pollfd &pollFD)
{
	if (pollFD.fd == -1)
		return ;
	Client		&client = _clients[pollFD.fd];
	auto		now		= std::chrono::system_clock::now();

	if (client.IsRegistered() == false
		&& client.getProgrammedDisconnection() > now + std::chrono::seconds(4)
		&& client.getProgrammedDisconnection() < now + std::chrono::seconds(10))
	{
		std::cout << "Client " << pollFD.fd << " timed out" << std::endl;
		client.addToSendBuffer(ERR_NOTREGISTERED(client.getNickname()));
		client.setProgrammedDisconnection(3);
	}
	if (client.getProgrammedDisconnection() < now + std::chrono::seconds(10))
		client.pingClient();
	if (client.getProgrammedDisconnection() < now)
	{
		pollFD.revents |= POLLERR;
		_disconnectClient(pollFD);
	}
}
