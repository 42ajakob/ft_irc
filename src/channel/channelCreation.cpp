/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelCreation.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 15:48:40 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/10 20:50:02 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

t_ChannelMap	Channel::_channels;

Channel &Channel::getChannel(string &name, Client &client)
{
	toLower(name);
	if (_channels.find(name) == _channels.end())
	{
		if (name[0] != '#')
			throw std::runtime_error(ERR_NOSUCHCHANNEL(name));
		t_ChannelCreatorKey	Key;
		_channels.emplace(std::piecewise_construct,
			std::forward_as_tuple(name),
			std::forward_as_tuple(Key, name, client));
	}
	return (_channels.at(name));
}

Channel &Channel::getChannel(string &name)
{
	toLower(name);
	if (_channels.find(name) == _channels.end())
		throw std::runtime_error(ERR_NOSUCHCHANNEL(name));
	return (_channels.at(name));
}
