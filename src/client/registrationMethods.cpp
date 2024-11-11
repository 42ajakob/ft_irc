/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   registrationMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 17:24:13 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 19:33:20 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"
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
	Server	&server = Server::getInstance();
	if (_isPasswordCorrect == false || _Nickname.empty() || _Username.empty())
		return ;
	_registered = true;
	_isPingSent = false;
	this->addToSendBuffer(RPL_WELCOME(_Nickname, _Nickname, _Username, _Hostname)
		+ RPL_YOURHOST(_Nickname)
		+ RPL_CREATED(_Nickname, server.getTimestamp())
		+ RPL_MYINFO(_Nickname, "o", "itkol"));
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
		throw std::invalid_argument(ERR_ALREADYREGISTRED(_Nickname));
	if (_isPasswordCorrect == false)
		throw std::invalid_argument(ERR_NOTREGISTERED(_Nickname));
	if (isNicknameValid(nickname) == false)
		throw std::invalid_argument(ERR_ERRONEUSNICKNAME(_Username, nickname));
	if (_isNicknameAvailable(nickname) == false)
		throw std::invalid_argument(ERR_NICKNAMEINUSE(_Username, nickname));
	_Nickname = std::move(nickname);
	_usedNicknames.insert(_Nickname);
	_markAsRegistered();
}

void	Client::setPasswordCorrect(bool isPasswordCorrect)
{
	if (_registered == true)
		throw std::invalid_argument(ERR_ALREADYREGISTRED(_Nickname));
	_isPasswordCorrect = isPasswordCorrect;
	if (_isPasswordCorrect == false)
		throw std::invalid_argument(ERR_PASSWDMISMATCH(_Nickname));
}

void	Client::setUsername(string &&username)
{
	if (_registered == true)
		throw std::invalid_argument(ERR_ALREADYREGISTRED(_Nickname));
	if (_isPasswordCorrect == false)
		throw std::invalid_argument(ERR_NOTREGISTERED(_Nickname));
	_Username = std::move(username);
	_markAsRegistered();
}
