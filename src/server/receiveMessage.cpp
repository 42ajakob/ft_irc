/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receiveMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 13:08:37 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 19:42:37 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include "Utils.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

static eCommand	checkForCommand(const string &line)
{
	string	command = line.substr(0, line.find(' '));
	toLower(command);
	const std::unordered_map<string, eCommand> commandMap = {
		{"ping",		eCommand::PING},
		{"pong",		eCommand::PONG},
		{"privmsg",		eCommand::PRIVMSG},
		{"join",		eCommand::JOIN},
		{"nick",		eCommand::NICK},
		{"user",		eCommand::USER},
		{"quit",		eCommand::QUIT},
		{"pass",		eCommand::PASS},
		{"cap",			eCommand::CAP},
		{"oper",		eCommand::OPER},
		{"rm_oper",		eCommand::RM_OPER},
		{"add_oper",	eCommand::ADD_OPER},
		{"ls_oper",		eCommand::LS_OPER},
		{"reload",		eCommand::RELOAD_SERVER},
		{"topic",		eCommand::TOPIC},
		{"kick",		eCommand::KICK},
		{"invite",		eCommand::INVITE},
		{"mode",		eCommand::MODE},
		{"bypass",		eCommand::BYPASS}
	};

	if (commandMap.find(command) != commandMap.end())
		return (commandMap.at(command));
	return (eCommand::UNKNOWN);
}

void	Server::_handlePing(Client &client, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	pong = "PONG";

	if (pos != string::npos)
		pong += " " + line.substr(pos);
	pong += "\r\n";
	client.addToSendBuffer(pong);
	if (client.IsRegistered() == true)
		client.setProgrammedDisconnection(TIMEOUT);
}

void	Server::_handleCap(Client &client, const string &line)
{
	if (line.find("LS") != string::npos)
		client.addToSendBuffer("CAP * LS :\r\n");
	if (line.find("REQ") != string::npos)
	{
		size_t	pos = findNextParameter(line, line.find("REQ"));
		pos = findNextParameter(line, pos);
		client.addToSendBuffer("CAP * NAK " + line.substr(pos) + "\r\n");
	}
}

void	Server::_executeCommand(const eCommand &command, string &line,
	const int &fd)
{
	const std::unordered_map<eCommand, t_Command> commandMap = {
		{eCommand::PING,		&Server::_handlePing},
		{eCommand::PONG,		&Server::_handlePong},
		{eCommand::PRIVMSG,		&Server::_handlePrivMsg},
		{eCommand::JOIN,		&Server::_handleJoin},
		{eCommand::NICK,		&Server::_handleNick},
		{eCommand::USER,		&Server::_handleUser},
		{eCommand::QUIT,		&Server::_handleQuit},
		{eCommand::PASS,		&Server::_handlePass},
		{eCommand::CAP,			&Server::_handleCap},
		{eCommand::TOPIC,		&Server::_handleTopic},
		{eCommand::KICK,		&Server::_handleKick},
		{eCommand::INVITE,		&Server::_handleInvite},
		{eCommand::OPER,		&Server::_Oper},
		{eCommand::RM_OPER,		&Server::_rmOper},
		{eCommand::ADD_OPER,	&Server::_addOper},
		{eCommand::LS_OPER,		&Server::_lsOper},
		{eCommand::BYPASS,		&Server::_OpBypass}
	};
	if (commandMap.find(command) != commandMap.end())
		(this->*(commandMap.at(command)))(_clients[fd], line);
	else if (command == eCommand::RELOAD_SERVER && _clients[fd].isOperator())
		reload();
	else if (command == eCommand::MODE)
		return ;
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
		if (command != eCommand::BYPASS)
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
