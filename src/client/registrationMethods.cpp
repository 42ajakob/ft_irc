/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:24:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/28 18:13:52 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <arpa/inet.h>

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

void	Client::setHostname(const sockaddr_in &clientAddr)
{
	string	ip_address = inet_ntoa(clientAddr.sin_addr);
	string	port = std::to_string(ntohs(clientAddr.sin_port));

	_Hostname = ip_address + ":" + port;
}

static void	toLower(string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

static bool	isNicknameValid(string &nickname)
{
	if (nickname.empty() || nickname[0] == ':' || nickname[0] == '#'
		|| nickname.find(' ') != string::npos)
		return (false);
	if (nickname.length() > 9)
		nickname.erase(9);
	return (true);
}

void	Client::setNickname(string &&nickname)
{
	if (_registered == true)
		throw std::invalid_argument("Client already registered");
	if (_isPasswordCorrect == false)
		throw std::invalid_argument("missing password");
	if (nickname.find(" ") == string::npos)
		throw std::invalid_argument("Missing nickname");
	nickname.erase(0, findNextParameter(nickname));
	if (nickname.empty())
		throw std::invalid_argument("Nickname cannot be empty");
	if (nickname[nickname.length() - 1] == '\r')
		nickname.erase(nickname.length() - 1, 1);
	toLower(nickname);
	if (isNicknameValid(nickname) == false)
		throw std::invalid_argument("Invalid nickname");
	if (_isNicknameAvailable(nickname) == false)
		throw std::invalid_argument("Nickname already in use");
	_Nickname = std::move(nickname);
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

void	Client::setUsername(string &&line)
{
	size_t		start;
	size_t		end;

	if (_registered == true)
		throw std::invalid_argument("Client already registered");
	if (_isPasswordCorrect == false)
		throw std::invalid_argument("missing password");
	start = findNextParameter(line);
	if (start == string::npos)
		throw std::invalid_argument("Missing username");
	end = findNextParameter(line, start);
	if (end == string::npos)
		end = line.length();
	line.erase(end);
	line.erase(0, start);
	_Username = std::move(line);
	_markAsRegistered();
}
