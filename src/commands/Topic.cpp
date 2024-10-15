/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:58:23 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/14 18:23:07 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::topic(Client &client, std::string topic)
{
	if (std::find(this->clients.begin(), this->clients.end(), client) == this->clients.end())
	{
		client.send(":%s 341 %s %s :You are not in this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	if (topic.empty())
	{
		client.send(":%s 341 %s %s :Current topic is: %s", SERVER_NAME, client.getNick().c_str(), this->name.c_str(), this->topic.c_str());
		return ;
	}
	if (client.getNick() != this->owner)
	{
		client.send(":%s 341 %s %s :You are not the owner of this channel", SERVER_NAME, client.getNick().c_str(), this->name.c_str());
		return ;
	}
	this->topic = topic;
	client.send(":%s 341 %s %s :Topic has been set to: %s", SERVER_NAME, client.getNick().c_str(), this->name.c_str(), this->topic.c_str());
}

