/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 17:21:54 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 20:56:15 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void	Channel::broadcastPrivMsg(const string &msg, const string &origin) const noexcept
{
	for (Client *member : _members)
	{
		if (member->getNickname() != origin)
			member->addToSendBuffer(":" + origin + " PRIVMSG " + _name + " :" + msg + "\r\n");
	}
}

void	Channel::_broadcastMsg(const string &msg, Client *client) const noexcept
{
	for (Client *member : _members)
	{
		if (member != client)
			member->addToSendBuffer(msg);
	}
}
