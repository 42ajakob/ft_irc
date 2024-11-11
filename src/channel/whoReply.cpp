/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whoReply.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 23:05:35 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/11 23:06:56 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sstream>

void	Channel::whoReply(Client &client) const
{
	string	flags = "H";

	for (const auto &member : _members)
	{
		if (member->isOperator())
			flags += "*";
		if (_operators.find(member) != _operators.end())
			flags += "@";
		client.addToSendBuffer(RPL_WHOREPLY(member->getNickname(), "",
			member->getUsername(), member->getHostname(), flags));
		flags = "H";
	}
}
