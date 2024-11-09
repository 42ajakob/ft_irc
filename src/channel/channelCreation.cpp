/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCreation.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:48:40 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/09 17:11:13 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

t_ChannelMap	Channel::_channels;

Channel &Channel::getChannel(const std::string &name, const Client &client)
{
	if (_channels.find(name) == _channels.end())
	{
		t_ChannelCreatorKey	Key;
		_channels.emplace(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(Key, name, client));
	}
	return (_channels.at(name));
}

Channel &Channel::getChannel(const std::string &name)
{
	if (_channels.find(name) == _channels.end())
		throw std::runtime_error("No such channel");
	return (_channels.at(name));
}
