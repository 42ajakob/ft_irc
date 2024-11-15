/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:03:30 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/15 13:49:26 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "numericReplies.hpp"
#include <stdexcept>

void	Channel::kick(const string &nick, Client &client, string& reason)
{
	Client		&target = Server::getInstance().getClientByNickname(nick);
	const auto	itMember = _members.find(&target);
	const auto	itOperator = _operators.find(&target);
	const auto	itInvited = _invited.find(&target);

	if (client.getNickname() == nick)
		throw std::invalid_argument("");
	if (itMember == _members.end())
		throw std::invalid_argument(ERR_USERNOTINCHANNEL(nick, _name));
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
	if (itOperator != _operators.end())
		_demoteClientFromOperator(client.getNickname(), target);
	if (itInvited != _invited.end())
		_invited.erase(itInvited);
	_members.erase(itMember);
	if (reason.empty())
		reason = client.getNickname();
	_relayMsg(":" + client.getFullname() + " KICK " + _name + " " + nick + " :" + reason + "\r\n", nullptr);
}
