/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainLoop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:42:24 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/18 12:26:35 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>
#include <array>

void Server::acceptClient(std::array<pollfd, BACKLOG_SIZE + 1> &pollFDs)
{
	int					clientFd;
	sockaddr_in			clientAddr;
	socklen_t			clientAddrLen = sizeof(clientAddr);

	clientFd = accept(_socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd == -1 && errno != EINTR)
		throw std::runtime_error("Error accepting a client connection");
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error setting the client socket to non-blocking");
	_clients[clientFd].setFd(clientFd);
	pollFDs[_clients.size()].fd = clientFd;
	pollFDs[_clients.size()].events = POLLIN | POLLHUP | POLLERR | POLLOUT;
	pollFDs[_clients.size()].revents = 0;
	std::cout << "Client " << clientFd << " connected" << std::endl;
}

void Server::disconnectClient(int clientFd)
{
	std::cout << "Client " << clientFd << " disconnected" << std::endl;
	close(clientFd);
	_clients.erase(clientFd);
}

void Server::start()
{
	static std::array<pollfd, BACKLOG_SIZE + 1>	pollFDs;

	pollFDs[0].fd = _socketFd;
	pollFDs[0].events = POLLIN;
	pollFDs[0].revents = 0;

	while (!_sig)
	{
		if (poll(pollFDs.data(), _clients.size() + 1, -1) == -1 && errno != EINTR)
			throw std::runtime_error(std::string("Poll Error: ") + strerror(errno));
		if (pollFDs[0].revents & POLLIN)
			acceptClient(pollFDs);
		for (size_t i = 1; i <= _clients.size(); i++)
		{
			// if (pollFDs[i].revents & POLLIN)
			// 	receiveMessage(pollFDs[i].fd);
			if (pollFDs[i].revents & POLLHUP || pollFDs[i].revents & POLLERR)
				disconnectClient(pollFDs[i].fd);
			// if (pollFDs[i].revents & POLLOUT)
			// 	sendMessage(pollFDs[i].fd);
		}
	}
}
