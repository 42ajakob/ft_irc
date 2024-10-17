/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/17 15:42:04 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::_sig = false;

Server::Server(std::string port, std::string password)
	: _password(password)
{
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
{}

void Server::init()
{
	const int	True = 1;
	_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketFd == -1 || fcntl(_socketFd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error creating the server socket");
	std::cout << "Server initialized" << std::endl;
	if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &True, sizeof(int)) == -1)
		throw std::runtime_error("Error setting the socket options");
	_serverAddr.sin_family = AF_INET;
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(_socketFd, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) == -1)
		throw std::runtime_error("Error binding the server socket");
	if (listen(_socketFd, BACKLOG_SIZE) == -1)
		throw std::runtime_error("Error listening on the server socket");
}

void Server::fdCloser()
{
	for (auto it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "Client " << it->first << " disconnected" << std::endl;
		close(it->first);
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
