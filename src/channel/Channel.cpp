/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:43:59 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/07 17:02:48 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include <algorithm>
#include <vector>

Channel::Channel(const t_ChannelCreatorKey &key, const string &name,
		const Client &creator) : Channel(name, creator)
{
	(void)key;
}

Channel::Channel(const string &name, const Client &creator)
	: _name(name), _userLimit(BACKLOG_SIZE)
{
	if (name.empty())
		throw std::invalid_argument("Channel name cannot be empty");
	_members.insert(&creator);
	_operators.insert(&creator);
}

Channel::~Channel()
{
	std::cout << "Channel " << _name << " destroyed" << std::endl;
}

void Channel::join(Client &client, const string &password)
{
	if (_mode.test(static_cast<size_t>(Mode::InviteOnly)) &&
		_invited.find(&client) == _invited.end())
		throw std::invalid_argument("Channel is invite only");
	if (_mode.test(static_cast<size_t>(Mode::PasswordProtected)) &&
		_password != password)
		throw std::invalid_argument("Invalid password");
	if (_mode.test(static_cast<size_t>(Mode::UserLimit)) &&
		_members.size() >= _userLimit)
		throw std::invalid_argument("Channel is full");
	_members.insert(&client);
	client.addToSendBuffer(":" +client.getNickname() + " JOIN " + _name + "\r\n");
	sendChannelInfo(client);
}

void Channel::sendChannelInfo(Client & client)
{
	//  SEND JOIN MESSAGE TO ALL MEMBERS
	if (_topic.empty())
		client.addToSendBuffer(":FT_IRC 331 " + client.getNickname() + " " + this->_name + " :No topic is set\r\n");
	else
		client.addToSendBuffer(":FT_IRC 332 " + client.getNickname() + " " + this->_name + " :" + this->_topic + "\r\n");
	client.addToSendBuffer(":FT_IRC 353 " + client.getNickname() + " = " + this->_name + " :");
	size_t	i = 0;
	for (const auto member : _members)
	{
		if (_operators.find(member) != _operators.end())
			client.addToSendBuffer("@");
		client.addToSendBuffer(member->getNickname());
		if (i++ < _members.size() - 1)
			client.addToSendBuffer(" ");
	}
	client.addToSendBuffer("\r\n");
	client.addToSendBuffer(":FT_IRC 366 " + client.getNickname() + " " + this->_name + " :End of /NAMES list\r\n");
}

void Channel::kick(const string &nickname)
{
	const Client &client = Server::getInstance().getClientByNickname(nickname);
	const auto it = std::find(_members.begin(), _members.end(), &client);

	if (it != _members.end())
		_members.erase(it);
}

void Channel::invite(const string &nickname)
{
	try
	{
		const Client &client = Server::getInstance().getClientByNickname(nickname);
		_invited.insert(&client);
	}
	catch (const std::invalid_argument &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void Channel::leave(const Client &client)
{
	_members.erase(&client);
	_operators.erase(&client);
}

void Channel::mode(Client &client)
{
	(void)client;
}

void Channel::clear()
{
	_members.clear();
}

bool Channel::operator==(const Channel &other) const
{
	return (_name == other._name);
}

bool Channel::operator==(const string &name) const
{
	return (_name == name);
}

void Channel::printMembers() const
{
	std::cout << "Channel " << _name << " members:" << std::endl;
	for (const Client *member : _members)
	{
		if (_operators.find(member) != _operators.end())
			std::cout << '@';
		std::cout << member->getNickname() << std::endl;
	}
}

void	Channel::clientDisconnected(const Client &client)
{
	auto it = _channels.begin();

	while (it != _channels.end())
	{
		Channel &channel = it->second;

		channel.leave(client);
		channel._invited.erase(&client);
		if (channel._members.empty())
		{
			it = _channels.erase(it);
			continue ;
		}
// Quickest solution, TBD if it's the best or it will randomly assign operators
		if (channel._operators.empty())
			channel._operators = channel._members;
		++it;
	}
}
