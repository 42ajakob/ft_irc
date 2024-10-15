/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:58:03 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/14 20:12:49 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::mode(Client &client, std::string mode)
{
	if (std::find(this->clients.begin(), this->clients.end(), client) == this->clients.end())
	{
		client.send(":%s 341 %s %s :You are not in this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	if (client.getNick() != this->owner)
	{
		client.send(":%s 341 %s %s :You are not the owner of this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	if (mode == "+i")
	{
		this->inviteOnly = true;
		client.send(":%s 341 %s %s :Channel is now invite only", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
	}
	else if (mode == "-i")
	{
		this->inviteOnly = false;
		client.send(":%s 341 %s %s :Channel is no longer invite only", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
	}
	else
	{
		client.send(":%s 341 %s %s :Unknown mode", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
	}
}
