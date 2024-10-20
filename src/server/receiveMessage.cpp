/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/20 16:26:21 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include "Utils.hpp"
#include <sstream>
#include <iostream>

eCommand	checkForCommand(const std::string &line)
{
	if (std::strncmp(line.c_str(), "PING", 4) == 0 && (line.size() == 4
		|| line[4] == ' '))
		return (eCommand::PING);
	if (std::strncmp(line.c_str(), "PRIVMSG", 7) == 0 && (line.size() == 7
		|| line[7] == ' '))
		return (eCommand::PRIVMSG);
	if (std::strncmp(line.c_str(), "JOIN", 4) == 0 && (line.size() == 4
		|| line[4] == ' '))
		return (eCommand::JOIN);
	if (std::strncmp(line.c_str(), ":bypass", 7) == 0)
		return (eCommand::DEBUG_BYPASS);
	return (eCommand::UNKNOWN);
}

void	executeCommand(eCommand command, std::string &line, pollfd &pollFD)
{
	switch (command)
	{
		case eCommand::PING:
			send(pollFD.fd, "PONG\n", 5, 0);
			break;
		case eCommand::PRIVMSG:
			break;
		case eCommand::JOIN:
			break;
		case eCommand::NICK:
			break;
		case eCommand::USER:
			break;
		case eCommand::QUIT:
			break;
		case eCommand::PASS:
			break;
		case eCommand::DEBUG_BYPASS:
			line += "\r\n";
			write(std::stoi(line.c_str() + 7), &line[9], line.size() - 8);
			std::cout << "Message sent to client " << std::stoi(line.c_str() + 7) << ": " << &line[8] << std::endl;
		default:
			break;
	}
}

void	Server::receiveMessage(pollfd &pollFD)
{
	static char			buffer[512];
	ssize_t				bytesRead;
	std::stringstream	ss;
	eCommand			command;
	std::string			line;

	bytesRead = recv(pollFD.fd, buffer + std::strlen(buffer), sizeof(buffer) - std::strlen(buffer), 0);
	if (bytesRead == -1)
		throw std::runtime_error("Error receiving message from client");
	if (bytesRead == 0)
		return (disconnectClient(pollFD));
	if (std::find(buffer, buffer + sizeof(buffer), '\n') == buffer + sizeof(buffer))
		return ;
	ss << buffer;
	std::getline(ss, line, '\n');
	while (line.size() > 0)
	{
		command = checkForCommand(line);
		if (command != eCommand::UNKNOWN)
			executeCommand(command, line, pollFD);
		else
			std::cout << "Received message from client " << pollFD.fd << ": " << line << std::endl;
		std::getline(ss, line, '\n');
	}
	std::memset(buffer, 0, sizeof(buffer));
}
