/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/25 16:16:56 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include "Client.hpp"

class Client;

class Server
{
private:
	int port;
	int sSocketFd;
	static bool sig;
	std::vector<Client> clients;
public:
	Server();
	// ~Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
	
	void init();
	void stop();
	void restart();
	void reload();
	void status();
	void fdCloser();
	void clear(int fd);
	void help();
};

#endif
