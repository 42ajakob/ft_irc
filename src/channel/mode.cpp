/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:00:07 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/17 16:24:23 by JFikents         ###   ########.fr       */
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

void	Channel::_handleOperatorMode(const string &targetNick, Client &client, bool isPromote)
{
	Client &target = Server::getInstance().getClientByNickname(targetNick);

	if (_members.find(&target) == _members.end())
		throw std::invalid_argument(ERR_USERNOTINCHANNEL(targetNick, _name));

	if (_operators.find(&target) == _operators.end() && isPromote)
		_promoteClientToOperator(client.getNickname(), target);
	else if (_operators.find(&target) != _operators.end())
		_demoteClientFromOperator(client.getNickname(), target);
}

void Channel::mode(const string &mode, Client &client, const string &mode_param)
{
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));

	if (mode_param.empty())
		throw std::invalid_argument(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
	else if (mode.find("o") != string::npos)
		_handleOperatorMode(mode_param, client, mode[0] == '+');
	else if (mode == "+k")
	{
		_password = mode_param;
		_mode.set(PasswordProtected);
	}
	else if (mode == "+l")
	{
		int newLimit = std::stoi(mode_param);
		
		if (newLimit < 1)
			return ;
		_userLimit = newLimit;
		_mode.set(UserLimit);
	}
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

	if (mode == "+i" && _mode.test(InviteOnly))
		throw std::invalid_argument(ERR_KEYSET(_name));
	else if (mode == "+i")
		_mode.set(InviteOnly);
	else if (mode == "-i")
		_mode.reset(InviteOnly);
	else if (mode == "+t")
		_mode.set(ProtectedTopic);
	else if (mode == "-t")
		_mode.reset(ProtectedTopic);
	else if (mode == "-l")
	{
		_userLimit = BACKLOG_SIZE;
		_mode.reset(UserLimit);
	}
	else if (mode == "-k")
	{
		_password.clear();
		_mode.reset(PasswordProtected);
	}
	else
		throw std::invalid_argument(ERR_UNKNOWNMODE(mode, _name));
}
