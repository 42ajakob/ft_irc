/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:16:37 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/09 18:17:17 by apeposhi         ###   ########.fr       */
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
		if (_members.find(&client) == _members.end())
			throw std::invalid_argument("You're not on that channel");
		if (_operators.find(&client) == _operators.end())
			throw std::invalid_argument("Cannot send invites");
		if (_members.find(&Server::getInstance().getClientByNickname(nickname)) != _members.end())
			throw std::invalid_argument("They are already on that channel");
		_invited.insert(&Server::getInstance().getClientByNickname(nickname));
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
		else
			client.addToSendBuffer(problem);
		std::cerr << "Error inviting user: " << problem << std::endl;
	}
}
