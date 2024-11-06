/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:44:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/06 16:54:57 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operator.hpp"
#include <fstream>
#include <iostream>

t_OperatorCredentials	Operator::_credentials;
t_StringSet				Operator::_loggedOperators;

Operator::Operator(string &&username, const string &password):
	_username(std::move(username))
{
	if (_credentials.empty())
		loadCredentials();
	if (_credentials.empty())
		throw std::runtime_error("No operator credentials found");
	if (_loggedOperators.find(_username) != _loggedOperators.end())
		throw std::runtime_error("Operator already logged in");
	if (_credentials.find(_username) == _credentials.end())
		throw std::runtime_error("Wrong operator credentials");
	if (_credentials[_username] != password)
		throw std::runtime_error("Wrong operator credentials");
	_loggedOperators.insert(_username);
	std::cout << "Operator " << _username << " logged in" << std::endl;
}

void	Operator::loadCredentials()
{
	std::ifstream	file(CREDENTIALS_FILE);
	string			username;
	string			password;

	if (!file.is_open())
		throw std::runtime_error("Error opening the operator credentials file");
	while (file >> username >> password)
		_credentials[username] = password;
	file.close();
}

Operator::~Operator()
{
	std::cout << "Operator " << _username << " logged out" << std::endl;
	_loggedOperators.erase(_username);
}
