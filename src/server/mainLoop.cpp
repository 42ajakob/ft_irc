/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainLoop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:42:24 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/22 20:44:15 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>
#include <array>
#include <cstring>

static void	debug_print_revents(short revents)
{
	if (revents & POLLOUT)
		std::cout << "POLLOUT ";
	if (revents & POLLIN)
		std::cout << "POLLIN ";
	if (revents & POLLHUP)
		std::cout << "POLLHUP ";
	if (revents & POLLERR)
		std::cout << "POLLERR ";
	std::cout << std::endl;
}

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
	_clients[clientFd].setHostname(inet_ntoa(clientAddr.sin_addr) + std::string(":") + std::to_string(ntohs(clientAddr.sin_port)));
	pollFDs[_clients.size()].fd = clientFd;
	pollFDs[_clients.size()].events = POLLIN | POLLHUP | POLLERR | POLLOUT;
	pollFDs[_clients.size()].revents = 0;
	std::cout << "Client " << clientFd << " connected" << std::endl;
}

void Server::disconnectClient(pollfd &pollFD)
{
	if (pollFD.fd == -1)
		return ;
	debug_print_revents(pollFD.revents);
	std::cout << "Client " << pollFD.fd << " disconnected" << std::endl;
	close(pollFD.fd);
	_clients.erase(pollFD.fd);
	pollFD.fd = -1;
	pollFD.revents = 0;
}

static void	init_pollFDs(std::array<pollfd, BACKLOG_SIZE + 1> &pollFDs,
	int socketFd)
{
	pollFDs[0].fd = socketFd;
	pollFDs[0].events = POLLIN;
	pollFDs[0].revents = 0;
	for (size_t i = 1; i < pollFDs.size(); i++)
	{
		pollFDs[i].fd = -1;
		pollFDs[i].events = POLLIN | POLLHUP | POLLERR | POLLOUT;
		pollFDs[i].revents = 0;
	}
}

void Server::start()
{
	static std::array<pollfd, BACKLOG_SIZE + 1>	pollFDs;

	if (pollFDs[0].fd != _socketFd)
		init_pollFDs(pollFDs, _socketFd);
	while (!_sig)
	{
		if (poll(pollFDs.data(), _clients.size() + 1, 0) == -1 && errno != EINTR)
			throw std::runtime_error(std::string("Poll Error: ") + strerror(errno));
		if (pollFDs[0].revents & POLLIN)
			acceptClient(pollFDs);
		for (size_t i = 1; i <= _clients.size(); i++)
		{
			if (pollFDs[i].revents & POLLIN)
				receiveMessage(pollFDs[i]);
			if (pollFDs[i].revents & POLLHUP || pollFDs[i].revents & POLLERR)
				disconnectClient(pollFDs[i]);
			if (pollFDs[i].revents & POLLOUT)
				sendMessage(pollFDs[i].fd);
			pollFDs[i].revents = 0;
			checkConnectionTimeout(pollFDs[i]);
		}
		pollFDs[0].revents = 0;
	}
}
