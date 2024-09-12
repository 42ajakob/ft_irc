/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:12:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/12 17:14:59 by apeposhi         ###   ########.fr       */
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