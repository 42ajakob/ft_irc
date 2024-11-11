/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:08:50 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 16:37:49 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

void	Server::_Oper(Client &client, string &line)
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

void	Server::_addOper(Client &client, string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				username;
	string				password;

	ss >> command >> username >> password;
	if (username.empty() || password.empty())
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "ADD_OPER"));
	else
		client.addOperator(username, password);
}

void	Server::_rmOper(Client &client, string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				username;

	ss >> command >> username;
	std::cout << "Removing operator " << username << std::endl;
	if (username.empty())
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), "RM_OPER"));
	else
		client.removeOperator(username);
}
