/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:00:07 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 18:44:06 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "numericReplies.hpp"
#include <stdexcept>

void	Channel::_promoteClientToOperator(const string &origin, Client &client)
{
	_mode.set(Mode::Operators);
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	client.addToSendBuffer(":" + origin + " MODE " + _name + " +o " + client.getNickname() + "\r\n");
	_operators.insert(&client);
}

void	Channel::_demoteClientFromOperator(const string &origin, Client &client)
{
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	client.addToSendBuffer(":" + origin + " MODE " + _name + " -o " + client.getNickname() + "\r\n");
	_operators.erase(&client);
}

void Channel::mode(const string &mode, Client &client, const string &mode_param)
{
	if (mode.size() >= 2 && mode[1] == 'o')
	{
		Client &target = Server::getInstance().getClientByNickname(mode_param);
		
		if (_members.find(&target) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
			else if (_operators.find(&target) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
			else if (_members.find(&target) == _members.end())
		throw std::invalid_argument(ERR_USERNOTINCHANNEL(target.getNickname(), _name));

		if (mode == "+o" && client.getNickname() != target.getNickname())
			_promoteClientToOperator(client.getNickname(), target);
		else if (mode == "-o" && client.getNickname() != target.getNickname())
			_demoteClientFromOperator(client.getNickname(), target);
	}
	
	if (mode == "+k")
	{
		_password = mode_param;
		_mode.set(PasswordProtected);
	}
	else if (mode == "-k")
	{
		_password.clear();
		_mode.reset(PasswordProtected);
	}
	else if (mode == "+l")
	{
		_userLimit = std::stoi(mode_param);
		_mode.set(UserLimit);
	}
	else if (mode == "-l")
	{
		_userLimit = BACKLOG_SIZE;
		_mode.reset(UserLimit);
	}
	else
		throw std::invalid_argument(ERR_UNKNOWNMODE(mode, _name));
}

static inline void	sendBanList(Client &client, string &channelName)
{
	client.addToSendBuffer(RPL_ENDOFBANLIST(channelName));
}

void Channel::mode(const string &mode, Client &client)
{
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	if (mode == "b")
		return (sendBanList(client, _name));
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));

	if (mode.empty()) {
		string modes = "+";
		string mode_params = _password + " "; 

		if (_mode.test(UserLimit))
			mode_params += std::to_string(_userLimit) + " ";
		for (size_t i = 0; i < ModeCount; i++)
			if (_mode.test(i))
				modes += "itkol"[i];
		client.addToSendBuffer(RPL_CHANNELMODEIS(client.getNickname(), _name, modes, mode_params));
	}
	else if (mode == "+i" && _mode.test(InviteOnly))
		throw std::invalid_argument(ERR_KEYSET(_name));
	else if (mode == "+i")
		_mode.set(InviteOnly);
	else if (mode == "-i")
		_mode.reset(InviteOnly);
	else if (mode == "+t")
		_mode.set(ProtectedTopic);
	else if (mode == "-t")
		_mode.reset(ProtectedTopic);
	else
		throw std::invalid_argument(ERR_UNKNOWNMODE(mode, _name));
}
