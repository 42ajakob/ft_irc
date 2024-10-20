/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/20 19:02:51 by JFikents         ###   ########.fr       */
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
	if (std::strncmp(line.c_str(), "NICK", 4) == 0 && (line.size() == 4
		|| line[4] == ' '))
		return (eCommand::NICK);
	if (std::strncmp(line.c_str(), "USER", 4) == 0 && (line.size() == 4
		|| line[4] == ' '))
		return (eCommand::USER);
	if (std::strncmp(line.c_str(), "QUIT", 4) == 0 && (line.size() == 4
		|| line[4] == ' '))
		return (eCommand::QUIT);
	if (std::strncmp(line.c_str(), "PASS", 4) == 0 && (line.size() == 4
		|| line[4] == ' '))
		return (eCommand::PASS);
	if (std::strncmp(line.c_str(), "CAP", 3) == 0 && (line.size() == 3
		|| line[3] == ' '))
		return (eCommand::CAP);
	if (std::strncmp(line.c_str(), ":bypass", 7) == 0)
		return (eCommand::DEBUG_BYPASS);
	return (eCommand::UNKNOWN);
}

void	debugBypass(std::string &line)
{
	line += "\r\n";
	try{
		write(std::stoi(line.c_str() + 7), &line[9], line.size() - 8);
		std::cout << "Message sent to client " << std::stoi(line.c_str() + 7) << ": " << &line[8] << std::flush;
	} catch (const std::exception &e) {
		std::cerr << "Error bypassing :" << e.what() << std::endl;
	}
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
		case eCommand::CAP:
			// Server::doCapNegotiation(pollFD.fd, line);
			break;
		case eCommand::DEBUG_BYPASS:
			debugBypass(line);
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
		if (command != eCommand::DEBUG_BYPASS)
			std::cout << "Received message from client " << pollFD.fd << ": " << line << std::endl;
		std::getline(ss, line, '\n');
	}
	std::memset(buffer, 0, sizeof(buffer));
}
