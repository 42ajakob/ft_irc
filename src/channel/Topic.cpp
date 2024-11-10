/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:58:23 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 16:00:48 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::topic(const string &topic, const Client &client)
{
	try {
		if (_members.find(&client) == _members.end())
			throw std::invalid_argument("You're not on that channel");
		if (_operators.find(&client) == _operators.end() && _mode.test(static_cast<size_t>(Mode::TopicProtected)))
			throw std::invalid_argument("Cannot change topic");
		_topic = topic;
	}
	catch (const std::exception &e)
	{
		string problem(e.what());

		if (problem == "You're not on that channel")
			client.addToSendBuffer(ERR_NOTONCHANNEL(_name));
		else if (problem == "Cannot change topic")
			client.addToSendBuffer(ERR_CHANOPRIVSNEEDED(_name));
		else
			client.addToSendBuffer(problem);
		std::cerr << "Error changing topic: " << problem << std::endl;
	}
}