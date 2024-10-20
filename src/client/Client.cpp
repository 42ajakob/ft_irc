/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/20 22:24:01 by JFikents         ###   ########.fr       */
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
	_usedNicknames.insert(_Nickname);
}

void Client::setHostname(std::string &Hostname)
{
	_Hostname = std::move(Hostname);
}

void Client::setUsername(std::string username)
{
	if (username.empty())
		throw std::invalid_argument("Username cannot be empty");
	_Username = username.substr(0, 9);
}

void Client::addToSendBuffer(std::string buffer)
{
	_sendBuffer += buffer;
}

void Client::addToRecvBuffer(std::string buffer)
{
	_recvBuffer += buffer;
}

void Client::markAsRegistered()
{
	_registered = true;
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
	return (_Hostname);
}

const std::string &Client::getSendBuffer() const
{
	return (_sendBuffer);
}

const std::string &Client::getRecvBuffer() const
{
	return (_recvBuffer);
}

const bool &Client::IsRegistered() const
{
	return (_registered);
}

void Client::clearSendBuffer()
{
	_sendBuffer.clear();
}

void Client::clearRecvBuffer()
{
	_recvBuffer.clear();
}

bool Client::operator==(const Client &other) const
{
	return (other.getNickname() == this->getNickname());
}
