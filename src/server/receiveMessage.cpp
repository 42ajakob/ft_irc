/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/07 15:19:13 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include "Utils.hpp"
#include <sstream>
#include <iostream>

static eCommand	checkForCommand(const string &line)
{
	string command = line.substr(0, line.find(' '));
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
	if (command == "OPER")
		return (eCommand::OPER);
	if (command == "RM_OPER")
		return (eCommand::RM_OPER);
	if (command == "ADD_OPER")
		return (eCommand::ADD_OPER);
	if (command == "LS_OPER")
		return (eCommand::LS_OPER);
	if (command == "RELOAD_SERVER")
		return (eCommand::RELOAD_SERVER);
	if (command == ":bypass")
		return (eCommand::DEBUG_BYPASS);
	return (eCommand::UNKNOWN);
}

void	Server::_debugBypass(string &line)
{
	const size_t	fd = line.find_first_of("0123456789");
	const size_t	msgStart = line.find_first_of(":", fd) + 1;
	int				clientFd;

	try{
		if (fd == string::npos)
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

void	Server::_Pong(const int &fd, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	pong = "PONG";

	if (pos != string::npos)
		pong += " " + line.substr(pos);
	pong += "\r\n";
	_clients[fd].addToSendBuffer(pong);
	if (_clients[fd].IsRegistered() == true)
		_clients[fd].setProgrammedDisconnection(TIMEOUT);
}

void	Server::_doCapNegotiation(const int &fd, string &line)
{
	if (line.find("LS") != string::npos)
		_clients[fd].addToSendBuffer("CAP * LS :\r\n");
	if (line.find("REQ") != string::npos)
	{
		size_t	pos = findNextParameter(line, line.find("REQ"));
		pos = findNextParameter(line, pos);
		_clients[fd].addToSendBuffer("CAP * NAK " + line.substr(pos) + "\r\n");
	}
}

void	Server::_checkPassword(const int &fd, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	password;

	if (pos == string::npos)
		return ;
	if (line[pos] == ':')
		pos++;
	password = line.substr(pos);
	_clients[fd].setPasswordCorrect(password == _password);
}

void	Server::_executeCommand(const eCommand &command, string &line,
	const int &fd)
{
	if (command == eCommand::PING)
		_Pong(fd, line);
	else if (command == eCommand::PONG)
		_clients[fd].resetPingTimerIfPongMatches(line);
	else if (command == eCommand::PRIVMSG)
		;
	else if (command == eCommand::JOIN)
		_joinChannel(fd, line);
	else if (command == eCommand::NICK)
		_clients[fd].setNickname(std::move(line));
	else if (command == eCommand::USER)
		_clients[fd].setUsername(std::move(line));
	else if (command == eCommand::QUIT)
		_quitClient(fd);
	else if (command == eCommand::PASS)
		_checkPassword(fd, line);
	else if (command == eCommand::CAP)
		_doCapNegotiation(fd, line);
	else if (command == eCommand::OPER)
		_Oper(fd, line);
	else if (command == eCommand::RM_OPER)
		_rmOper(fd, line);
	else if (command == eCommand::ADD_OPER)
		_addOper(fd, line);
	else if (command == eCommand::LS_OPER)	
		_clients[fd].listOperators();
	else if (command == eCommand::RELOAD_SERVER && _clients[fd].isOperator())
		reload();
	else if (command == eCommand::DEBUG_BYPASS)
		_debugBypass(line);
}

void	Server::_parseMessage(const int &fd)
{
	eCommand			command;
	std::stringstream	ss;
	string				line;

	ss << _clients[fd].getRecvBuffer();
	_clients[fd].clearRecvBuffer();
	std::getline(ss, line, '\n');
	while (line.size() > 0)
	{
		if (line[line.size() - 1] == '\r')
			line.erase(line.size() - 1, 1);
		command = checkForCommand(line);
		if (command != eCommand::DEBUG_BYPASS)
			std::cout << "Received message from client " << fd << ": " << line << std::endl;
		try {
			_executeCommand(command, line, fd);
		}
		catch (const std::exception &e) {
			std::cerr << "Error executing command: " << e.what() << std::endl;
		}
		std::getline(ss, line, '\n');
	}
}

void	Server::_receiveMessage(pollfd &pollFD)
{
	char	buffer[512];
	ssize_t	bytesRead;

	std::memset(buffer, 0, sizeof(buffer));
	bytesRead = recv(pollFD.fd, buffer, sizeof(buffer), 0);
	if (bytesRead == -1)
		throw std::runtime_error("Error receiving message from client");
	if (bytesRead == 0)
		return (_disconnectClient(pollFD));
	_clients[pollFD.fd].addToRecvBuffer(string(buffer, bytesRead));
	if (_clients[pollFD.fd].getRecvBuffer().find("\n") == string::npos)
		return ;
	_parseMessage(pollFD.fd);
}
