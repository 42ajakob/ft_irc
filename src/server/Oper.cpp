/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:08:50 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/06 20:19:17 by JFikents         ###   ########.fr       */
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
		_clients[fd].addToSendBuffer(":FT_IRC 461 " + _clients[fd].getNickname() + " ADD_OPER :Not enough parameters\r\n");
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
		_clients[fd].addToSendBuffer(":FT_IRC 461 " + _clients[fd].getNickname() + " RM_OPER :Not enough parameters\r\n");
	else
		_clients[fd].removeOperator(username);
}
