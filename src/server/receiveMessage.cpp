/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/22 19:49:37 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include "Utils.hpp"
#include <sstream>
#include <iostream>

static eCommand	checkForCommand(const std::string &line)
{
	std::string command = line.substr(0, line.find(' '));
	if (command == "PING")
		return (eCommand::PING);
	if (command == "PONG")
		return (eCommand::PONG);
	if (command == "PRIVMSG")
		return (eCommand::PRIVMSG);
	if (command == "JOIN")
		return (eCommand::JOIN);
	if (command == "NICK")
		return (eCommand::NICK);
	if (command == "USER")
		return (eCommand::USER);
	if (command == "QUIT")
		return (eCommand::QUIT);
	if (command == "PASS")
		return (eCommand::PASS);
	if (command == "CAP")
		return (eCommand::CAP);
	if (command == ":bypass")
		return (eCommand::DEBUG_BYPASS);
	return (eCommand::UNKNOWN);
}

void	Server::debugBypass(std::string &line)
{
	const auto	fd = line.find_first_of("0123456789");
	const auto	msgStart = line.find_first_of(":", fd) + 1;
	int			clientFd;

	try{
		if (fd == std::string::npos)
			return ;
		line += "\r\n";
		clientFd = std::stoi(&line[fd]);

		_clients[clientFd].addToSendBuffer(&line[msgStart]);
		std::cout << "Bypass message sent to client " << clientFd;
		std::cout << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << "Error bypassing :" << e.what() << std::endl;
	}
}

void	Server::Pong(const int &fd, const std::string &line)
{
	auto		pos = line.find_first_of(":");
	std::string	pong = "PONG";

	if (pos != std::string::npos)
		pong += " " + line.substr(pos);
	pong += "\n";
	_clients[fd].addToSendBuffer(pong);
}

void	Server::doCapNegotiation(int fd, std::string &line)
{
	if (line.find("LS") != std::string::npos)
		_clients[fd].addToSendBuffer("CAP * LS :\r\n");
}

void	Server::checkPassword(const int fd, const std::string &line)
{
	size_t	pos = line.find_first_of(" ");
	std::string password;

	if (pos == std::string::npos)
		return ;
	if (line.find_first_of(":") > pos && line.find_first_of(":") != std::string::npos)
		pos = line.find_first_of(":");
	pos++;
	password = line.substr(pos);
	if (password[password.length() - 1] == '\r')
		password.erase(password.length() - 1, 1);
	_clients[fd].setPasswordCorrect(password == _password);
}

void	Server::executeCommand(const eCommand &command, std::string &line, const pollfd &pollFD)
{
	try{
	switch (command)
	{
		case eCommand::PING:
			Pong(pollFD.fd, line);
			break;
		case eCommand::PONG:
			_clients[pollFD.fd].resetPingTimer(line);
			break;
		case eCommand::PRIVMSG:
			break;
		case eCommand::JOIN:
			break;
		case eCommand::NICK:
			_clients[pollFD.fd].setNickname(line);
			break;
		case eCommand::USER:
			_clients[pollFD.fd].setUsername(line);
			break;
		case eCommand::QUIT:
			break;
		case eCommand::PASS:
			checkPassword(pollFD.fd, line);
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
	catch (const std::exception &e) {
		std::cerr << "Error executing command: " << e.what() << std::endl;
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
	ssize_t			bytesRead;

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
