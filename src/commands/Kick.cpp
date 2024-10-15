/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:57:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/14 20:16:10 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::kick(Client &client, std::string target)
{
	if (std::find(this->clients.begin(), this->clients.end(), client) == this->clients.end())
	{
		client.send(":%s 341 %s %s :You are not in this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	if (client.getNick() != this->owner && client.getNick() != target)
	{
		client.send(":%s 341 %s %s :You are not the owner of this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	std::vector<Client>::iterator it = std::find_if(this->clients.begin(), this->clients.end(), [&target](Client &c) { return c.getNick() == target; });
	if (it == this->clients.end())
	{
		client.send(":%s 341 %s %s :User %s is not in this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str(), target.c_str());
		return ;
	}
	this->clients.erase(it);
	client.send(":%s 341 %s %s :You have kicked %s from channel %s", SERVER_NAME, client.getNick().c_str(), this->name.c_str(), target.c_str(), this->name.c_str());
}
