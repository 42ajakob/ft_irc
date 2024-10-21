/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:24:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/21 20:56:18 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <stdexcept>
#include <algorithm>
#include <cctype>

static void	toLower(std::string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

const bool &Client::IsRegistered() const
{
	return (_registered);
}

void Client::_markAsRegistered()
{
	if (_isPasswordCorrect == false || _Nickname.empty() || _Username.empty())
		return ;
	_registered = true;
}

void Client::setPasswordCorrect(bool isPasswordCorrect)
{
	if (_registered == true)
		throw std::invalid_argument("Client already registered");
	_isPasswordCorrect = isPasswordCorrect;
	_markAsRegistered();
}

const bool &Client::IsPasswordCorrect() const
{
	return (_isPasswordCorrect);
}

bool	Client::isNicknameAvailable(std::string nickname)
{
	return (_usedNicknames.find(nickname) == _usedNicknames.end());
}

bool	isNicknameValid(std::string &nickname)
{
	if (nickname.empty())
		return (false);
	if (nickname.length() > 9)
		nickname = nickname.substr(0, 9);
	if (nickname[0] == ':' || nickname[0] == '#'
		|| nickname.find(' ') != std::string::npos)
		return (false);
	return (true);
}

void Client::setNickname(std::string nickname)
{
	if (nickname.empty())
		throw std::invalid_argument("Nickname cannot be empty");
	if (nickname.find(" ") == std::string::npos)
		throw std::invalid_argument("Missing nickname");
	nickname = nickname.substr(nickname.find_first_of(" ") + 1);
	toLower(nickname);
	if (isNicknameValid(nickname) == false)
		throw std::invalid_argument("Invalid nickname");
	if (isNicknameAvailable(nickname) == false)
		throw std::invalid_argument("Nickname already in use");
	_Nickname = nickname;
	_usedNicknames.insert(_Nickname);
	_markAsRegistered();
}

void Client::setHostname(std::string &&Hostname)
{
	_Hostname = std::move(Hostname);
}

void Client::setUsername(std::string line)
{
	std::string	username;
	size_t		start;
	size_t		end;

	if (line.empty())
		throw std::invalid_argument("Username cannot be empty");
	start = line.find(" ");
	if (start == std::string::npos)
		throw std::invalid_argument("Missing username");
	end = line.find(" ", ++start);
	if (end == std::string::npos)
		end = line.length();
	username = line.substr(start, end - start);
	_Username = username;
	this->setHostname("localhost");
	if (end != line.length())
	{
		start = line.find("0", end);
		if (start == std::string::npos)
			this->setHostname("localhost");
		start = line.find(" ", start);
		end = line.find(" ", ++start);
		this->setHostname(line.substr(start, end - start));
	}
	_markAsRegistered();
}
