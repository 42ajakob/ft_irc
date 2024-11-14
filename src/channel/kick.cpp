/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:03:30 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/14 17:42:55 by JFikents         ###   ########.fr       */
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

	if (itMember == _members.end())
		throw std::invalid_argument(ERR_USERNOTINCHANNEL(nick, _name));
	if (_operators.find(&client) == _operators.end())
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
	if (itOperator != _operators.end())
		_demoteClientFromOperator(client.getNickname(), target);
	if (reason.empty())
		reason = client.getNickname();
	_relayMsg(":" + client.getFullname() + " KICK " + _name + " " + nick + " :" + reason + "\r\n", nullptr);
	_members.erase(itMember);
	if (itInvited != _invited.end())
		_invited.erase(itInvited);
}
