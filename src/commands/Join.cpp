/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:19:30 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/08 13:20:02 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::join(Client &client)
{
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
	client.send(":%s 341 %s %s :You have joined channel %s", SERVER_NAME, client.getNick().c_str(), this->name.c_str(), this->name.c_str());
}
