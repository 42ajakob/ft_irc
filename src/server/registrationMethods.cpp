/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:59:59 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 19:35:52 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>

void	Server::_handleUser(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				username;

	ss >> command >> username;
	(void)command;
	try {
		if (username.empty())
			throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"));
		client.setUsername(std::move(username));
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}

void	Server::_handleNick(Client &client, const string &line)
{
	std::stringstream	ss(line);
	string				command;
	string				nickname;

	ss >> command >> nickname;
	(void)command;
	try {
		toLower(nickname);
		if (nickname.empty())
			throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getUsername(), "NICK"));
		client.setNickname(std::move(nickname));
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}

void	Server::_handlePass(Client &client, const string &line)
{
	size_t	pos = findNextParameter(line);
	string	password;

	try {
		if (pos == string::npos)
			throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
		if (line[pos] == ':')
			pos++;
		password = line.substr(pos);
		if (password.empty())
			throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getNickname(), "PASS"));
		client.setPasswordCorrect(password == _password);
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}
