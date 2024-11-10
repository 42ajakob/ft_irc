/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:43:59 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 16:40:45 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
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
		throw std::invalid_argument(ERR_NOSUCHCHANNEL(name));
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
		throw std::invalid_argument(ERR_INVITEONLYCHAN(client.getNickname(), _name));
	if (_mode.test(static_cast<size_t>(Mode::PasswordProtected)) &&
		_password != password)
		throw std::invalid_argument(ERR_BADCHANNELKEY(client.getNickname(), _name));
	if (_mode.test(static_cast<size_t>(Mode::UserLimit)) &&
		_members.size() >= _userLimit)
		throw std::invalid_argument(ERR_CHANNELISFULL(client.getNickname(), _name));
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

void Channel::kick(const string &nickname, const Client &client)
{
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
	
	const auto itMember = _members.find(&Server::getInstance().getClientByNickname(nickname));
	const auto itOperator = _operators.find(&Server::getInstance().getClientByNickname(nickname));
	const auto itInvited = _invited.find(&Server::getInstance().getClientByNickname(nickname));

	if (itMember != _members.end())
		_members.erase(itMember);
	if (itOperator != _operators.end())
		_operators.erase(itOperator);
	if (itInvited != _invited.end())
		_invited.erase(itInvited);
}

void Channel::leave(const Client &client)
{
	_members.erase(&client);
	_operators.erase(&client);
}

void Channel::mode(string mode, Client &client, const Client &nick)
{
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));

	if (mode == "+o" && client.getNickname() != nick.getNickname())
		_operators.insert(&nick);
	else if (mode == "-o" && client.getNickname() != nick.getNickname())
		_operators.erase(&nick);
	else
		throw std::invalid_argument(ERR_UNKNOWNMODE(mode, _name));
}

void Channel::mode(string mode, Client &client)
{
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));

	if (mode == "+i")
		_mode.set(static_cast<size_t>(Mode::InviteOnly));
	else if (mode == "-i")
		_mode.reset(static_cast<size_t>(Mode::InviteOnly));
	else if (mode == "+t")
		_mode.set(static_cast<size_t>(Mode::ProtectedTopic));
	else if (mode == "-t")
		_mode.reset(static_cast<size_t>(Mode::ProtectedTopic));
	else if (mode == "+k")
		_mode.set(static_cast<size_t>(Mode::PasswordProtected));
	else if (mode == "-k")
		_mode.reset(static_cast<size_t>(Mode::PasswordProtected));
	else if (mode == "+l")
		_mode.set(static_cast<size_t>(Mode::UserLimit));
	else if (mode == "-l")
		_mode.reset(static_cast<size_t>(Mode::UserLimit));
	else
		throw std::invalid_argument(ERR_UNKNOWNMODE(mode, _name));
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
