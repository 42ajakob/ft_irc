/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleMode.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:11:10 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/13 17:36:42 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "numericReplies.hpp"
#include <sstream>

void	Server::_handleMode(Client &client, string const &line)
{
	stringstream	ss(line);
	string			command;
	string			channelName;
	string			mode;
	string			nick;

	ss >> command >> channelName >> mode >> nick;

	if ((nick.empty() && (mode == "+o" || mode == "-o"))
		|| channelName.empty())
	{
		client.addToSendBuffer(ERR_NEEDMOREPARAMS(client.getNickname(), command));
		return ;
	}
	try {
		Channel	&channel = Channel::getChannel(channelName);
		if (!nick.empty())
			channel.mode(mode, client, nick);
		else
			channel.mode(mode, client);
	}
	catch (const std::invalid_argument &e) {
		_logError(client, e.what());
	}
}
