/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:58:23 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/13 19:02:49 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::topic(string &topic, Client &client)
{
	const string topicReply =
		_topic.empty() ? RPL_NOTOPIC(client.getNickname(), _name)
			: RPL_TOPIC(client.getNickname(), _name, _topic);

	if (topic.empty())
		return (client.addToSendBuffer(topicReply));
	if (_members.find(&client) == _members.end())
		throw std::invalid_argument(ERR_NOTONCHANNEL(_name));
	if (_operators.find(&client) == _operators.end() && _mode.test(static_cast<size_t>(Mode::ProtectedTopic)))
		throw std::invalid_argument(ERR_CHANOPRIVSNEEDED(_name));
	if (topic[0] == ':')
		topic.erase(0, 1);
	std::cout << "New topic for " << _name << ": " << topic << std::endl;
	_topic = topic;
}
