/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:44:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/14 19:56:16 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Operator.hpp"
#include <fstream>
#include <iostream>
#include "Server.hpp"

t_OperatorCredentials	Operator::_credentials;
t_StringSet				Operator::_loggedOperators;

Operator::Operator(string &&username, const string &password):
	_username(std::move(username))
{
	if (_credentials.empty())
		loadCredentials();
	if (_credentials.empty())
		throw std::runtime_error(ERR_NOOPERHOST());
	if (_loggedOperators.find(_username) != _loggedOperators.end())
		throw std::runtime_error(ERR_ALREADYREGISTRED(_username));
	if (_credentials.find(_username) == _credentials.end())
		throw std::runtime_error(ERR_PASSWDMISMATCH(_username));
	if (_credentials[_username] != password)
		throw std::runtime_error(ERR_PASSWDMISMATCH(_username));
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

void	Operator::addOperator(const string &username, const string &password)
{
	std::ofstream	file(CREDENTIALS_FILE, std::ios::app);

	if (!file.is_open())
		throw std::runtime_error("Error opening the operator credentials file");
	file << '\n' << username << ' ' << password << std::flush;
	file.close();
	_credentials[username] = password;
}

void	Operator::removeOperator(const string &username)
{
	if (_credentials.find(username) == _credentials.end())
		throw std::runtime_error("Operator not found");
	_credentials.erase(_credentials.find(username));

	std::ofstream	file(CREDENTIALS_FILE);

	if (!file.is_open())
		throw std::runtime_error("Error opening the operator credentials file");
	for (auto login : _credentials)
		file << login.first << ' ' << login.second << std::endl;
	file.close();
}

const string	&Operator::getUsername() const noexcept
{
	return (_username);
}

const string	Operator::listOperators() const noexcept
{
	string	ret;

	for (auto &it : _credentials)
		ret += it.first + ' ';
	return (ret);
}
