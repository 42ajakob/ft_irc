/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:57:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/04 14:46:41 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::kick(Client &client, std::string target)
{
	if (_members.find(&client) == _members.end())
		throw std::runtime_error("You are not in this channel");

	if (_members.find(target) == _members.end())
		throw std::runtime_error("Target is not in this channel");

	_members.erase(target);
	target->addToSendBuffer("KICK " + _name + " " + target->getNickname() + "\r\n");
}
