/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:43:59 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/17 16:49:41 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "numericReplies.hpp"

Channel::Channel(const t_ChannelCreatorKey &key, const string &name,
		Client &creator): Channel(name, creator)
{
	// The key is used to allow a constructor to be public while still
	// being unaccessible from outside the class.
	// The actual constructor is private [Channel(const string &, Client &)]
	(void)key;
}

Channel::Channel(const string &name, Client &creator)
	: _name(name), _userLimit(BACKLOG_SIZE)
{
	if (name.empty())
		throw std::invalid_argument(ERR_NOSUCHCHANNEL(name));
	_members.insert(&creator);
}

Channel::~Channel()
{
	std::cout << "Channel " << _name << " destroyed" << std::endl;
}

bool	Channel::operator==(const Channel &other) const
{
	return (_name == other._name);
}

bool	Channel::operator==(const string &name) const
{
	return (_name == name);
}

void	Channel::clientDisconnected(Client &client)
{
	auto	it = _channels.begin();

	while (it != _channels.end())
	{
	try {
		Channel &channel = it->second;

		channel.part(client, "Client disconnected");
		if (channel._members.empty())
		{
			it = _channels.erase(it);
			continue ;
		}
		if (channel._operators.empty())
			channel._promoteClientToOperator("FT_IRC", **channel._members.begin());
	}
	catch (const std::invalid_argument &e) {
		(void)e;
	}
		++it;
	}
}

std::ostream	&Channel::insertInfoToOutstream(std::ostream &os) const
{
	os << "Channel " << _name << " members:" << _getMembersList();
	return (os);
}

std::ostream	&operator<<(std::ostream &os, const Channel &channel)
{
	channel.insertInfoToOutstream(os);
	return (os);
}

void		Channel::cleanUnusedChannels()
{
	for (auto it = _channels.begin(); it != _channels.end();)
	{
		if (it->second._members.empty())
			it = _channels.erase(it);
		else
			++it;
	}
}
