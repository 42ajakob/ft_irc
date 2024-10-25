/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/25 19:54:47 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include "Client.hpp"
# include "Channel.hpp"
# include "Utils.hpp"
# include <vector>
# include <unordered_map>
# include <sys/socket.h>
# include <sys/types.h> // getaddrinfo, bind, connect
# include <arpa/inet.h> // htons, htonl, ntohl, ntohs, inet_addr, inet_ntoa
# include <fcntl.h>
# include <poll.h>
# include <unistd.h>
# include <array>

# define BACKLOG_SIZE 512

using std::unordered_map;

class Client;
class Channel;

class Server
{
	private:
		int							_port;
		int							_socketFd;
		static bool					_sig;
		unordered_map<int, Client>	_clients;
		std::vector<Channel>		_channels;
		std::string					_password;
		sockaddr_in					_serverAddr;

		void	acceptClient(std::array<pollfd, BACKLOG_SIZE + 1> &pollFDs);
		void	receiveMessage(pollfd &pollFD);
		void	disconnectClient(pollfd &pollFD);
		void	sendMessage(int fd);
		void	parseMessage(const pollfd &pollFD);
		void	executeCommand(const eCommand &command, std::string &line,
			const pollfd &pollFD);
		void	debugBypass(std::string &line);
		void	Pong(const int &fd, const std::string &line);
		void	doCapNegotiation(int fd, std::string &line);
		void	checkConnectionTimeout(pollfd &pollFD);
		void	checkPassword(const int fd, const std::string &line);

	public:
		const Client &getClientByNickname(const std::string &nickname) const;

		Server()								= delete;
		Server(const Server &other)				= delete;
		Server &operator=(const Server &other)	= delete;
		Server(std::string port, std::string password);
		~Server();
		
		void	initSocket();
		void	start();
		void	stop();
		void	restart();
		void	reload();
		void	status();
		void	fdCloser();
		void	help();
};

#endif
