/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:43:59 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/26 22:05:10 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
}

Channel::~Channel()
{
}

Channel::Channel(const Channel &other)
{
	(void)other;
}

Channel &Channel::operator=(const Channel &other)
{
	(void)other;
	return (*this);
}

void Channel::join(Client &client)
{
	clients.push_back(client);
}

void Channel::kick(Client &client)
{
	auto it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
		clients.erase(it);
}

void Channel::invite(Client &client)
{
	clients.push_back(client);
}

void Channel::leave(Client &client)
{
	auto it = std::find(clients.begin(), clients.end(), client);
	if (it != clients.end())
		clients.erase(it);
}

void Channel::mode(Client &client)
{
	(void)client;
}

void Channel::send(Client &client, std::string message)
{
	(void)client;
	std::cout << message << std::endl;
}

void Channel::sendAll(std::string message)
{
	for (int i = 0; i < clients.size(); i++)
	{
		std::cout << message << std::endl;
	}
}

void Channel::clear()
{
	clients.clear();
}