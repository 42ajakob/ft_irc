/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/24 15:43:55 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <stdexcept>
#include <chrono>

std::unordered_set<string> Client::_usedNicknames;

Client::Client() :
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

const string	&Client::getNickname() const
{
	return (_Nickname);
}

const string	&Client::getUsername() const
{
	return (_Username);
}

const string	&Client::getHostname() const
{
	return (_Hostname);
}

bool	Client::operator==(const Client &other) const
{
	return (other.getNickname() == this->getNickname());
}
