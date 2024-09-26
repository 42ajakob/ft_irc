/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/26 21:03:27 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::clear(int fd) {
	auto it_fds = std::remove_if
}

Server::Server()
{
	fd = -1;
}

Server::~Server()
{}

Server::Server(const Server &other)
{
	(void)other;
}

bool Server::sig = false;

Server &Server::operator=(const Server &other)
{
	(void)other;
	return (*this);
}

void Server::fdCloser()
{
	for (int i = 0; i < clients.size(); i++)
	{
		std::cout << "Client " << clients[i].getFd() << " disconnected" << std::endl;
		close(fd[i]);
	}
}

void Server::init()
{
	this->port = 4224;
	// create server socket WIP;
	std::cout << "Server initialized" << std::endl;
	while (server::sig == false)
	{
		// accept client connections WIP;
	}
}

void Server::start()
{
	std::cout << "Server started" << std::endl;
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
