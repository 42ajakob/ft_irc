/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:00:07 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/15 14:27:01 by ajakob           ###   ########.fr       */
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
	std::cout << "Mode " << mode << " Client " << client.getNickname() << " modeparam " << mode_param << std::endl;
	if (mode.size() >= 2 && mode[1] == 'o')
	{
		Client &target = Server::getInstance().getClientByNickname(mode_param);
		
		if (_members.find(&target) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
			else if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
			else if (_members.find(&target) == _members.end())
		throw std::invalid_argument(ERR_USERNOTINCHANNEL(target.getNickname(), _name));

		if (mode == "+o" && client.getNickname() != target.getNickname())
			_promoteClientToOperator(client.getNickname(), target);
		else if (mode == "-o" && client.getNickname() != target.getNickname())
			_demoteClientFromOperator(client.getNickname(), target);
	}
	else if (mode == "+k")
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

void	Channel::sendModes(Client &client) const
{
	string modes = "+";
	string mode_params = _password + " "; 

	if (_mode.test(UserLimit))
		mode_params += std::to_string(_userLimit) + " ";
	for (size_t i = 0; i < ModeCount; i++)
		if (_mode.test(i))
			modes += "itkol"[i];
	client.addToSendBuffer(RPL_CHANNELMODEIS(client.getNickname(), _name, modes, mode_params));
}

void Channel::mode(const string &mode, Client &client)
{
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));

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
