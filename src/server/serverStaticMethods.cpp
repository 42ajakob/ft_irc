/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverStaticMethods.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 14:24:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/10 18:38:25 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <algorithm>
#include <stdexcept>
#include "Client.hpp"

Client &Server::getClientByNickname(const string &nickname)
{
	auto it = std::find_if(_clients.begin(), _clients.end(),
		[&nickname](std::pair<const int, Client> &client)
		{
			return (client.second == nickname);
		});
	if (it != _clients.end())
		return (it->second);
	throw std::invalid_argument(ERR_NOSUCHNICK(nickname));
}
