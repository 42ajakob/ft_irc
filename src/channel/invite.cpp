/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:16:37 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/15 14:50:19 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include "numericReplies.hpp"
#include <iostream>
#include <stdexcept>

void Channel::invite(const string &nickname, Client &client)
{
	try {
		auto target = &Server::getInstance().getClientByNickname(nickname);
		
		if (_members.find(&client) == _members.end())
			throw std::invalid_argument("You're not on that channel");
		if (_operators.find(&client) == _operators.end())
			throw std::invalid_argument("Cannot send invites");
		if (_members.find(target) != _members.end())
			throw std::invalid_argument("They are already on that channel");
		if (_invited.find(target) != _invited.end())
			throw std::invalid_argument("You're already invited");
		_invited.insert(target);
		target->addToSendBuffer(RPL_INVITING(_name, client.getNickname()));
	}
	catch (const std::exception &e)
	{
		string problem(e.what());

		if (problem == "You're not on that channel")
			client.addToSendBuffer(ERR_NOTONCHANNEL(_name));
		else if (problem == "Cannot send invites")
			client.addToSendBuffer(ERR_CHANOPRIVSNEEDED(_name));
		else if (problem == "They are already on that channel")
			client.addToSendBuffer(ERR_USERONCHANNEL(client.getNickname(), nickname, _name));
		std::cerr << "Error inviting user: " << problem << std::endl;
	}
}
