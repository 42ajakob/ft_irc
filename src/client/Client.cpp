/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/17 15:20:49 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <algorithm>

std::vector<std::string> Client::_usedNicknames;

Client::Client()
{}

Client::~Client()
{}

int Client::getFd() const {
	return (_fd);
}

void Client::setFd(int fd_value) {
	_fd = fd_value;
}

bool	Client::isNicknameAvailable(std::string nickname)
{
	const auto it =
		std::find(_usedNicknames.begin(), _usedNicknames.end(), nickname);

	return (it == _usedNicknames.end());
}

void Client::setNickname(std::string nickname)
{
	if (nickname.empty())
		throw std::invalid_argument("Nickname cannot be empty");
	if (isNicknameAvailable(nickname) == false)
		throw std::invalid_argument("Nickname already in use");
	_nickname = nickname.substr(0, 9);
	_usedNicknames.push_back(nickname);
}

void Client::setUserName(std::string username)
{
	if (username.empty())
		throw std::invalid_argument("Username cannot be empty");
	_userName = username.substr(0, 9);
}

const std::string &Client::getNickname() const
{
	return (_nickname);
}

bool Client::operator ==(const Client &other) const
{
	return (other.getNickname() == this->getNickname());
}
