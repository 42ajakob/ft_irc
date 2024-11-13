/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 17:21:54 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 19:37:31 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void	Channel::PrivMsg(const string &msg, const Client &origin) const noexcept
{
	string	PrivMsg;

	PrivMsg = ":" + origin.getNickname() + " PRIVMSG " + _name + " :" + msg + "\r\n";
	_relayMsg(PrivMsg, &origin);
}

void	Channel::_relayMsg(const string &msg, const Client *client) const noexcept
{
	for (Client *member : _members)
	{
		if (member != client)
			member->addToSendBuffer(msg);
	}
}
