/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:43:59 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/17 15:20:28 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include <algorithm>

std::vector<std::string> Channel::_usedNames;

bool Channel::isNameAvailable(std::string name)
{
	const auto it = std::find(_usedNames.begin(), _usedNames.end(), name);

	return (it == _usedNames.end());
}

Channel::Channel(std::string name, const Client &creator, const Server &server)
	: _name(name), _userLimit(BACKLOG_SIZE), _server(&server)
{
	if (name.empty())
		throw std::invalid_argument("Channel name cannot be empty");
	if (isNameAvailable(name) == false)
		throw std::invalid_argument("Channel name already in use");
	_members.push_back(&creator);
	_operators.push_back(&creator);
	_usedNames.push_back(name);
}

Channel::~Channel()
{
}

void Channel::join(Client &client)
{
	if (_mode.test(static_cast<size_t>(Mode::UserLimit)) &&
		_members.size() >= _userLimit)
		throw std::invalid_argument("Channel is full");
	_members.push_back(&client);
}

void Channel::kick(const std::string &nickname)
{
	const Client &client = _server->getClientByNickname(nickname);
	const auto it = std::find(_members.begin(), _members.end(), &client);

	if (it != _members.end())
		_members.erase(it);
}

void Channel::invite(const std::string &nickname)
{
	const Client &client = _server->getClientByNickname(nickname);
	_members.push_back(&client);
}

void Channel::leave(Client &client)
{
	auto it = std::find(_members.begin(), _members.end(), &client);
	if (it != _members.end())
		_members.erase(it);
}

void Channel::mode(Client &client)
{
	(void)client;
}

void Channel::clear()
{
	_members.clear();
}