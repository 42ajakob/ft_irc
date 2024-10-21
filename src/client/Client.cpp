/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/21 17:48:08 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <stdexcept>
#include <chrono>

std::unordered_set<std::string> Client::_usedNicknames;

Client::Client() : _connectionTime(std::chrono::system_clock::now()),
	_programmedDisconnection(std::chrono::system_clock::now() + std::chrono::seconds(60))
{
	static bool reserveMemory = true;

	if (reserveMemory == true)
		_usedNicknames.reserve(512), reserveMemory = false;
}

Client::~Client()
{
	_usedNicknames.erase(_Nickname);
}

void	Client::setFd(int fd_value)
{
	_fd = fd_value;
}

const int	&Client::getFd() const
{
	return (_fd);
}

const std::string	&Client::getNickname() const
{
	return (_Nickname);
}

const std::string	&Client::getUsername() const
{
	return (_Username);
}

const std::string	&Client::getIpAddr() const
{
	return (_Hostname);
}

bool	Client::operator==(const Client &other) const
{
	return (other.getNickname() == this->getNickname());
}
