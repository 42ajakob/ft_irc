/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:59:59 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 18:39:23 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

// IT ISN'T FINISHED
void	Server::_handleUser(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				username;

	ss >> command >> username;
	(void)command;
	username = line;
	client.setUsername(std::move(username));
}

// IT ISN'T FINISHED
void	Server::_handleNick(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				nickname;

	ss >> command >> nickname;
	(void)command;
	nickname = line;
	client.setNickname(std::move(nickname));
}

void	Server::_handlePass(Client &client, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	password;

	if (pos == string::npos)
		return ;
	if (line[pos] == ':')
		pos++;
	password = line.substr(pos);
	client.setPasswordCorrect(password == _password);
}
