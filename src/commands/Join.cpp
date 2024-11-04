/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 13:19:30 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/04 14:42:46 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void Channel::join(Client &client, const string &password)
{
	if (_password != password)
		throw std::runtime_error("Invalid password");

	if (_members.find(&client) != _members.end())
		throw std::runtime_error("Already in channel");

	_members.insert(&client);
	client.addToSendBuffer("JOIN " + _name + "\r\n");
}
