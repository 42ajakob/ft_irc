/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:06:59 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/15 16:24:13 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "numericReplies.hpp"
#include <string>

void	Channel::part(Client &client, const string &reason)
{
	string	replyMsg;

	replyMsg = ":" + client.getNickname() + " PART " + _name;
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	if (!reason.empty())
		replyMsg +=" :" + reason;
	_relayMsg(replyMsg + "\r\n", nullptr);
	_members.erase(&client);
	if (_members.empty())
		return (_channels.erase(_name), void());
	_operators.erase(&client);
	if (_operators.empty())
		_promoteClientToOperator("FT_IRC", **_members.begin());
	_invited.erase(&client);
}
