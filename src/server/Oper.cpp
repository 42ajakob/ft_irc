/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 18:08:50 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/05 16:11:13 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
