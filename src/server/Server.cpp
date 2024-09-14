/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/14 12:50:19 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{}

Server::~Server()
{}

Server::Server(const Server &other)
{
	(void)other;
}

Server &Server::operator=(const Server &other)
{
	(void)other;
	return (*this);
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
