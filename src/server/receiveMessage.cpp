/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/21 13:27:14 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include "Utils.hpp"
#include <sstream>
#include <iostream>

static eCommand	checkForCommand(const std::string &line)
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

void	Server::debugBypass(std::string &line)
{
	line += "\r\n";
	try{
		_clients[std::stoi(line.c_str() + 7)].addToSendBuffer(&line[9]);
		std::cout << "Message sent to client " << std::stoi(line.c_str() + 7) << ": " << &line[9] << std::flush;
	} catch (const std::exception &e) {
		std::cerr << "Error bypassing :" << e.what() << std::endl;
	}
}

void	Server::Pong(int fd, std::string &line)
{
	line = "PONG" + line.substr(4) + "\n";
	_clients[fd].addToSendBuffer(line);
}

void	Server::doCapNegotiation(int fd, std::string &line)
{
	if (line.find("LS") != std::string::npos)
		_clients[fd].addToSendBuffer("CAP * LS :\r\n");
}

void	Server::executeCommand(const eCommand &command, std::string &line, const pollfd &pollFD)
{
	switch (command)
	{
		case eCommand::PING:
			Pong(pollFD.fd, line);
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
			doCapNegotiation(pollFD.fd, line);
			break;
		case eCommand::DEBUG_BYPASS:
			debugBypass(line);
		default:
			break;
	}
}

void	Server::parseMessage(const pollfd &pollFD)
{
	eCommand			command;
	std::stringstream	ss;
	std::string			line;

	ss << _clients[pollFD.fd].getRecvBuffer();
	_clients[pollFD.fd].clearRecvBuffer();
	std::getline(ss, line, '\n');
	while (line.size() > 0)
	{
		command = checkForCommand(line);
		if (command != eCommand::DEBUG_BYPASS)
			std::cout << "Received message from client " << pollFD.fd << ": " << line << std::endl;
		if (command != eCommand::UNKNOWN)
			executeCommand(command, line, pollFD);
		std::getline(ss, line, '\n');
	}
}

void	Server::receiveMessage(pollfd &pollFD)
{
	char			buffer[512];
	ssize_t				bytesRead;

	std::memset(buffer, 0, sizeof(buffer));
	bytesRead = recv(pollFD.fd, buffer, sizeof(buffer), 0);
	if (bytesRead == -1)
		throw std::runtime_error("Error receiving message from client");
	if (bytesRead == 0)
		return (disconnectClient(pollFD));
	_clients[pollFD.fd].addToRecvBuffer(std::string(buffer, bytesRead));
	if (_clients[pollFD.fd].getRecvBuffer().find("\n") == std::string::npos)
		return ;
	parseMessage(pollFD);
}
