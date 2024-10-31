/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/31 14:30:35 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::_sig = false;
std::unique_ptr<Server> Server::_instance = nullptr;

Server::Server(const string &port, const string &&password): _password(std::move(password))
{
	if (_instance != nullptr)
		throw std::runtime_error("Attempt to create a second server instance");
	try{
		this->_port = std::stoi(port);
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	std::cout << "Server created" << std::endl;
}

Server::~Server()
{
	for (const auto &[fd, client] : _clients)
	{
		disconnectClient(_pollFDs[fd]);
	}
	std::cout << "Server destroyed" << std::endl;
}

Server	&Server::getInstance(const string &port, const string &&password)
{
	if (_instance == nullptr)
	{
		if (port.empty() || password.empty())
			throw std::runtime_error("Server instance not created");
		_instance = std::unique_ptr<Server>(new Server(port, std::move(password)));
	}
	return (*_instance);
}

void Server::initSocket()
{
	const int	True = 1;
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1 || fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error creating the server socket");
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &True, sizeof(int)) == -1)
		throw std::runtime_error("Error setting the socket options");
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	#ifdef __APPLE__
		_serverAddr.sin_len = sizeof(_serverAddr);
	#endif
	if (bind(_socketFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
		throw std::runtime_error("Error binding the server socket");
	if (listen(_socketFd, BACKLOG_SIZE) == -1)
		throw std::runtime_error("Error listening on the server socket");
	std::cout << "Server socket created" << std::endl;
}

void Server::fdCloser()
{
	for (const auto &[fd, client] : _clients)
	{
		std::cout << "Client " << fd << " disconnected" << std::endl;
		close(fd);
	}
}

void Server::stop()
{
	std::cout << "Server stopped" << std::endl;
}

void Server::restart()
{
	std::cout << "Server restarted" << std::endl;
}

void Server::reload()
{
	std::cout << "Server reloaded" << std::endl;
}

void Server::status()
{
	std::cout << "Server status" << std::endl;
}

void Server::help()
{
	std::cout << "Server help" << std::endl;
}
