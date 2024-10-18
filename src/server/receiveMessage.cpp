/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/18 16:44:52 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>

void	Server::receiveMessage(pollfd &pollFD)
{
	char		buffer[512];
	ssize_t		bytesRead;

	std::memset(buffer, 0, sizeof(buffer));
	bytesRead = recv(pollFD.fd, buffer, sizeof(buffer), 0);
	if (bytesRead == -1)
		throw std::runtime_error("Error receiving message from client");
	if (bytesRead == 0)
	{
		disconnectClient(pollFD);
		return ;
	}
	std::cout << "Received message from client " << pollFD.fd << ": " << buffer << std::flush;
}
