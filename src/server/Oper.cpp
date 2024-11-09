/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:08:50 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/09 14:05:46 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

void	Server::_Oper(const int &fd, string &line)
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
	_clients[fd].giveOperatorAccess(std::move(username), password);
}

void	Server::_addOper(const int &fd, string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				username;
	string				password;

	ss >> command >> username >> password;
	if (username.empty() || password.empty())
		_clients[fd].addToSendBuffer(ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), "ADD_OPER"));
	else
		_clients[fd].addOperator(username, password);
}

void	Server::_rmOper(const int &fd, string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				username;

	ss >> command >> username;
	std::cout << "Removing operator " << username << std::endl;
	if (username.empty())
		_clients[fd].addToSendBuffer(ERR_NEEDMOREPARAMS(_clients[fd].getNickname(), "RM_OPER"));
	else
		_clients[fd].removeOperator(username);
}
