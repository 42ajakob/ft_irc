/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:18:05 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/08 13:19:19 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::invite(Client &client)
{
	if (client.getNick() == this->owner)
	{
		client.send(":%s 341 %s %s :You are already the owner of this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	if (std::find(this->clients.begin(), this->clients.end(), client) != this->clients.end())
	{
		client.send(":%s 341 %s %s :You are already in this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	if (this->inviteOnly)
	{
		if (std::find(this->invited.begin(), this->invited.end(), client.getNick()) == this->invited.end())
		{
			client.send(":%s 341 %s %s :You are not invited to this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
			return ;
		}
	}
	this->clients.push_back(client);
	client.send(":%s 341 %s %s :You have been invited to channel %s", SERVER_NAME, client.getNick().c_str(), this->name.c_str(), this->name.c_str());
}

