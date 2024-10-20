/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/20 20:39:30 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

std::unordered_set<std::string> Client::_usedNicknames;

Client::Client()
{
	static bool reserveMemory = true;

	if (reserveMemory == true)
		_usedNicknames.reserve(512), reserveMemory = false;
}

Client::~Client()
{
	_usedNicknames.erase(_Nickname);
}

bool	Client::isNicknameAvailable(std::string nickname)
{
	return (_usedNicknames.find(nickname) == _usedNicknames.end());
}

void Client::setFd(int fd_value) {
	_fd = fd_value;
}

void Client::setNickname(std::string nickname)
{
	if (nickname.empty())
		throw std::invalid_argument("Nickname cannot be empty");
	if (isNicknameAvailable(nickname) == false)
		throw std::invalid_argument("Nickname already in use");
	_Nickname = nickname.substr(0, 9);
	_usedNicknames.insert(nickname);
}

void Client::setIpAddr(std::string ipAddr)
{
	_ipAddr = ipAddr;
}

void Client::setUsername(std::string username)
{
	if (username.empty())
		throw std::invalid_argument("Username cannot be empty");
	_Username = username.substr(0, 9);
}

void Client::setSendBuffer(std::string buffer)
{
	_sendbuffer = buffer;
}

void Client::setRecvBuffer(std::string buffer)
{
	_recvBuffer = buffer;
}

const int &Client::getFd() const
{
	return (_fd);
}

const std::string &Client::getNickname() const
{
	return (_Nickname);
}

const std::string	&Client::setUsername() const
{
	return (_Username);
}

const std::string &Client::getIpAddr() const
{
	return (_ipAddr);
}

const std::string &Client::getSendBuffer() const
{
	return (_sendbuffer);
}

const std::string &Client::getRecvBuffer() const
{
	return (_recvBuffer);
}

void Client::clearSendBuffer()
{
	_sendbuffer.clear();
}

void Client::clearRecvBuffer()
{
	_recvBuffer.clear();
}

bool Client::operator==(const Client &other) const
{
	return (other.getNickname() == this->getNickname());
}
