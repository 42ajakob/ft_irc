/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:06:59 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/15 19:05:58 by ajakob           ###   ########.fr       */
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
		return ;
	_operators.erase(&client);
	if (_operators.empty())
		_promoteClientToOperator("FT_IRC", **_members.begin());
	_invited.erase(&client);
}
