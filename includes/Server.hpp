/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/08 21:02:08 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include "Client.hpp"
# include "Channel.hpp"
# include <vector>
# include <map>

# define BACKLOG_SIZE 512

class Client;
class Channel;

class Server
{
private:
	int						_port;
	int						_serverSocketFd;
	static bool				_sig;
	std::map<int, Client>	_clients;
	std::vector<Channel>	_channels;
	std::string				_password;
public:
	Server()								= delete;
	Server(const Server &other)				= delete;
	Server &operator=(const Server &other)	= delete;
	Server(int port, std::string password);
	~Server();
	
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
