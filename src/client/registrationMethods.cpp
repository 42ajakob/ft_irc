/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:24:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/22 20:56:06 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <stdexcept>
#include <algorithm>
#include <cctype>

bool	Client::_isNicknameAvailable(string nickname)
{
	return (_usedNicknames.find(nickname) == _usedNicknames.end());
}

void	Client::_markAsRegistered()
{
	if (_isPasswordCorrect == false || _Nickname.empty() || _Username.empty())
		return ;
	_registered = true;
	_isPingSent = false;
	this->addToSendBuffer(":FT_IRC 001 " + _Nickname + " :Welcome to the FT_IRC Network " + _Nickname + "!" + _Username + "@" + _Hostname + "\r\n");
}

const bool &Client::IsPasswordCorrect() const
{
	return (_isPasswordCorrect);
}

const bool &Client::IsRegistered() const
{
	return (_registered);
}

void	Client::setHostname(string &&Hostname)
{
	_Hostname = std::move(Hostname);
}

static void	toLower(string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

static bool	isNicknameValid(string &nickname)
{
	if (nickname.empty())
		return (false);
	if (nickname[nickname.length() - 1] == '\r')
		nickname = nickname.substr(0, nickname.length() - 1);
	if (nickname.length() > 9)
		nickname = nickname.substr(0, 9);
	if (nickname[0] == ':' || nickname[0] == '#'
		|| nickname.find(' ') != string::npos)
		return (false);
	return (true);
}

void	Client::setNickname(string nickname)
{
	if (_registered == true)
		throw std::invalid_argument("Client already registered");
	if (_isPasswordCorrect == false)
		throw std::invalid_argument("missing password");
	if (nickname.empty())
		throw std::invalid_argument("Nickname cannot be empty");
	if (nickname.find(" ") == string::npos)
		throw std::invalid_argument("Missing nickname");
	nickname = nickname.substr(nickname.find_first_of(" ") + 1);
	toLower(nickname);
	if (isNicknameValid(nickname) == false)
		throw std::invalid_argument("Invalid nickname");
	if (_isNicknameAvailable(nickname) == false)
		throw std::invalid_argument("Nickname already in use");
	_Nickname = nickname;
	_usedNicknames.insert(_Nickname);
	_markAsRegistered();
}

void	Client::setPasswordCorrect(bool isPasswordCorrect)
{
	if (_registered == true)
		throw std::invalid_argument("Client already registered");
	_isPasswordCorrect = isPasswordCorrect;
	if (_isPasswordCorrect == false)
		this->addToSendBuffer(":FT_IRC 464 * :Password incorrect\r\n");
}

void	Client::setUsername(string line)
{
	string	username;
	size_t		start;
	size_t		end;

	if (_registered == true)
		throw std::invalid_argument("Client already registered");
	if (_isPasswordCorrect == false)
		throw std::invalid_argument("missing password");
	if (line.empty())
		throw std::invalid_argument("Username cannot be empty");
	start = line.find(" ");
	if (start == string::npos)
		throw std::invalid_argument("Missing username");
	end = line.find(" ", ++start);
	if (end == string::npos)
		end = line.length();
	username = line.substr(start, end - start);
	_Username = username;
	_markAsRegistered();
}
