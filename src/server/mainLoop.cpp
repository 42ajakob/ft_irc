/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainLoop.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 15:42:24 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/25 20:11:20 by JFikents         ###   ########.fr       */
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

void Server::acceptClient()
{
	int			clientFd;
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	const auto	clientPollFD = std::find_if(_pollFDs.begin(), _pollFDs.end(), [](const pollfd &pollFD) { return pollFD.fd == -1; });

	clientFd = accept(_socketFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
	if (clientFd == -1 && errno != EINTR)
		throw std::runtime_error("Error accepting a client connection");
	clientPollFD->fd = clientFd;
	_clients[clientFd].setHostname(inet_ntoa(clientAddr.sin_addr) + std::string(":") + std::to_string(ntohs(clientAddr.sin_port)));
	clientPollFD->revents = 0;
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error setting the client socket to non-blocking");
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

void	Server::initPollFDs()
{
	_pollFDs[0].fd = _socketFd;
	_pollFDs[0].events = POLLIN;
	_pollFDs[0].revents = 0;
	for (size_t i = 1; i < _pollFDs.size(); i++)
	{
		_pollFDs[i].fd = -1;
		_pollFDs[i].events = POLLIN | POLLHUP | POLLERR | POLLOUT;
		_pollFDs[i].revents = 0;
	}
}

void Server::start()
{
	while (!_sig)
	{
		if (poll(_pollFDs.data(), _clients.size() + 1, 0) == -1 && errno != EINTR)
			throw std::runtime_error(std::string("Poll Error: ") + strerror(errno));
		if (_pollFDs[0].revents & POLLIN)
			acceptClient();
		for (size_t i = 1; i <= _clients.size(); i++)
		{
			if (_pollFDs[i].revents & POLLIN)
				receiveMessage(_pollFDs[i]);
			if (_pollFDs[i].revents & POLLHUP || _pollFDs[i].revents & POLLERR)
				disconnectClient(_pollFDs[i]);
			if (_pollFDs[i].revents & POLLOUT)
				sendMessage(_pollFDs[i].fd);
			_pollFDs[i].revents = 0;
			checkConnectionTimeout(_pollFDs[i]);
		}
		_pollFDs[0].revents = 0;
	}
}
