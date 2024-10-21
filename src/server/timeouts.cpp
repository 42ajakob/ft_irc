/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeouts.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:19:28 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/21 21:13:52 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <chrono>

void	Server::checkConnectionTimeout(pollfd &pollFD)
{
	if (pollFD.fd == -1)
		return ;
	Client		&client = _clients[pollFD.fd];
	auto		now		= std::chrono::system_clock::now();
	auto		elapsed = now - _clients[pollFD.fd].getConnectionTime();
	auto		elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed);

	if (elapsedSeconds > std::chrono::seconds(10) && client.IsRegistered() == false
		&& client.getProgrammedDisconnection() > now + std::chrono::seconds(30))
	{
		std::cout << "Client " << pollFD.fd << " timed out" << std::endl;
		client.addToSendBuffer("ERROR :Closing Link: " + client.getNickname() + " (Failed to register in Time)\r\n");
		client.setProgrammedDisconnection(std::chrono::seconds(5));
	}
	if (client.getProgrammedDisconnection() < now + std::chrono::seconds(3))
		client.pingClient();
	if (client.getProgrammedDisconnection() < now)
	{
		pollFD.revents |= POLLERR;
		disconnectClient(pollFD);
	}
}
