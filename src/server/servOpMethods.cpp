/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:08:50 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 14:14:11 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

void	Server::_Oper(Client &client, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	username;
	string	password;

	if (pos == string::npos)
		return ;
	username = line.substr(pos, line.find(' ', pos) - pos);
	if ((pos = findNextParameter(line, pos)) == string::npos)
		return ;
	password = line.substr(pos);
	client.giveOperatorAccess(std::move(username), password);
}

void	Server::_addOper(Client &client, const string &line)
{
	stringstream	ss(line);
	string			command;
	string			username;
	string			password;

	ss >> command >> username >> password;
	if (username.empty() || password.empty())
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "ADD_OPER"));
	else
		client.addOperator(username, password);
}

void	Server::_rmOper(Client &client, const string &line)
{
	stringstream	ss(line);
	string			command;
	string			username;

	ss >> command >> username;
	std::cout << "Removing operator " << username << std::endl;
	if (username.empty())
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "RM_OPER"));
	else
		client.removeOperator(username);
}

void	Server::_OpBypass(Client &client, const string &line)
{
	const size_t	fd = line.find_first_of("0123456789");
	const size_t	msgStart = line.find_first_of(":", fd) + 1;

	try{
		if (fd == string::npos || msgStart == string::npos)
			return ;
		const int	clientFd = std::stoi(&line[fd]);
		string		msg = line.substr(msgStart) + "\r\n";

		_clients[clientFd].addToSendBuffer(msg);
		std::cout << "Bypass message sent to client " << clientFd;
		std::cout << std::endl;
		msg = "Msg bypassed to client " + std::to_string(clientFd) + "\r\n";
		client.addToSendBuffer(msg);
	}
	catch (const std::exception &e) {
		string error = "Error bypassing :" + string(e.what());
		_logError(client, error);
	}
}

void	Server::_lsOper(Client &client, const string &line)
{
	(void)line;
	client.listOperators();
}
