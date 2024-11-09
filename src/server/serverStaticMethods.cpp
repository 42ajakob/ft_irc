/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverStaticMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:24:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/09 12:47:58 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include <stdexcept>
#include "Client.hpp"

const Client &Server::getClientByNickname(const string &nickname) const
{
	auto it = std::find_if(_clients.begin(), _clients.end(),
		[&nickname](const std::pair<const int, Client> &client)
		{
			return (client.second == nickname);
		});
	if (it != _clients.end())
		return (it->second);
	throw std::invalid_argument(ERR_NOSUCHNICK(nickname));
}
