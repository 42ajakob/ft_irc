/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:43:59 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/11 21:29:51 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include <vector>

Channel::Channel(const t_ChannelCreatorKey &key, const string &name,
		Client &creator) : Channel(name, creator)
{
	(void)key;
}

Channel::Channel(const string &name, Client &creator)
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

void	Channel::join(Client &client, const string &password)
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
	_sendChannelInfo(client);
}

string	Channel::_getMembersList() const noexcept
{
	string membersList;
	size_t i = 0;

	for (const auto member : _members)
	{
		if (_operators.find(member) != _operators.end())
			membersList += "@";
		membersList += member->getNickname();
		if (i++ < _members.size() - 1)
			membersList += " ";
	}
	return (membersList);
}

void	Channel::_sendChannelInfo(Client &client)
{
	if (_topic.empty())
		client.addToSendBuffer(RPL_NOTOPIC(client.getNickname(), _name));
	else
		client.addToSendBuffer(RPL_TOPIC(client.getNickname(), _name, _topic));
	client.addToSendBuffer(RPL_NAMREPLY(client.getNickname(), _name, _getMembersList()));
	client.addToSendBuffer(RPL_ENDOFNAMES(client.getNickname(), _name));
	_broadcastMsg(":" + client.getNickname() + " JOIN " + _name + "\r\n", &client);
}

void	Channel::kick(const string &nickname, Client &client)
{
	Client		&target = Server::getInstance().getClientByNickname(nickname);
	const auto	itMember = _members.find(&target);
	const auto	itOperator = _operators.find(&target);
	const auto	itInvited = _invited.find(&target);

	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
	if (itMember != _members.end())
		_members.erase(itMember);
	if (itOperator != _operators.end())
		_operators.erase(itOperator);
	if (itInvited != _invited.end())
		_invited.erase(itInvited);
}

void	Channel::mode(Client &client)
{
	(void)client;
}

bool	Channel::operator==(const Channel &other) const
{
	return (_name == other._name);
}

bool	Channel::operator==(const string &name) const
{
	return (_name == name);
}

void	Channel::printMembers() const
{
	std::cout << "Channel " << _name << " members:" << _getMembersList() << std::endl;
}

void	Channel::clientDisconnected(Client &client)
{
	auto it = _channels.begin();

	while (it != _channels.end())
	{
		Channel &channel = it->second;

		channel.part(client, "Client disconnected");
		if (channel._members.empty())
		{
			it = _channels.erase(it);
			continue ;
		}
		if (channel._operators.empty())
			channel._operators = channel._members;
		++it;
	}
}

const string	&Channel::getName() const noexcept
{
	return (_name);
}

void	Channel::part(Client &client, const string &reason)
{
	_broadcastMsg(":" + client.getNickname() + " PART " + _name + " :" + reason + "\r\n", &client);
	_members.erase(&client);
	_operators.erase(&client);
	_invited.erase(&client);
}
