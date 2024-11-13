/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:06:59 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 19:28:45 by JFikents         ###   ########.fr       */
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
	_operators.erase(&client);
	_invited.erase(&client);
}
