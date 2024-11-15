/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendMessage.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 22:51:52 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 16:08:15 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <sstream>

void	Server::_sendClientBuffer(const int &fd)
{
	ssize_t	bytesSent;
	string	buffer;

	buffer = _clients[fd].getSendBuffer();
	if (buffer.empty())
		return ;
	bytesSent = send(fd, buffer.c_str(), buffer.size(), 0);
	if (bytesSent == -1)
		throw std::runtime_error("Error sending message to client");
	std::cout << "Sending to client " << fd << ": " << buffer << std::flush;
	_clients[fd].clearSendBuffer();
}
