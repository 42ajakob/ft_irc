/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleKick.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:34:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/13 18:57:08 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>


void	Server::_handleKick(Client &client, const string &line)
{
	stringstream	ss(line);
	string			channelName;
	string			command;
	string			nickname;
	string			reason;

	/*
	The reason is not properly extracted from the stringstream, for now it is
		just a word, but it should be the rest of the line.
	There is an example of how to extract the rest of the line in
		_handlePart() or _handlePrivMsg().
	The nickname can be multiple clients separated by commas.
		See Server::_handleJoin() for an example on how to split a string.
	*/
	ss >> command >> channelName >> nickname >> reason;

	toLower(nickname);

	if (channelName.empty() || nickname.empty())
	{
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), command));
		return ;
	}
	try {
		Channel::getChannel(channelName).kick(nickname, client);
		/*
		It should be sent to the channel, not the client, and the origin should
			be the client, not the server. I added a "fix" in the Channel::kick()
			method, but it is missing the reason.

		The reason is optional, so it should be checked if it is empty before
			sending it.
		See Channel::part() for an example.
		*/
		client.addToSendBuffer(":FT_IRC KICK " + channelName + " " + nickname + " :" + reason + "\r\n");
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}
