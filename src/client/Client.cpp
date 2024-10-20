/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/20 19:05:09 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>

std::vector<std::string> Client::_usedNicknames;

Client::Client()
{}

Client::~Client()
{}

bool	Client::isNicknameAvailable(std::string nickname)
{
	const auto it =
		std::find(_usedNicknames.begin(), _usedNicknames.end(), nickname);

	return (it == _usedNicknames.end());
}

void Client::setFd(int fd_value) {
	_fd = fd_value;
}

void Client::setNickName(std::string nickname)
{
	if (nickname.empty())
		throw std::invalid_argument("Nickname cannot be empty");
	if (isNicknameAvailable(nickname) == false)
		throw std::invalid_argument("Nickname already in use");
	_nickName = nickname.substr(0, 9);
	_usedNicknames.push_back(nickname);
}

void Client::setIpAddr(std::string ipAddr)
{
	_ipAddr = ipAddr;
}

void Client::setUserName(std::string username)
{
	if (username.empty())
		throw std::invalid_argument("Username cannot be empty");
	_userName = username.substr(0, 9);
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
	return (_nickName);
}

const std::string	&Client::setUserName() const
{
	return (_userName);
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
