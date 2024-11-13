/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timeouts.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:19:28 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 14:15:38 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <chrono>
#include <sstream>

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

void	Server::_handlePong(Client &client, const string &line)
{
	stringstream	ss(line);
	string			command;
	string			response;

	ss >> command >> response;
	if (response[0] == ':')
		response.erase(0, 1);
	client.resetPingTimerIfPongMatches(response);
}
