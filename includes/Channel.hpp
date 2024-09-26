/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:45:05 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/26 22:03:47 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include "Client.hpp"

class Client;

class Channel
{
private:
	std::string name;
	std::vector<Client> clients;
public:
	Channel();
	~Channel();
	Channel(const Channel &other);
	Channel &operator=(const Channel &other);
	
	void join(Client &client);
	void kick(Client &client);
	void invite(Client &client);
	void leave(Client &client);
	void mode(Client &client);
	void send(Client &client, std::string message);
	void sendAll(std::string message);
	void clear();
};

#endif
