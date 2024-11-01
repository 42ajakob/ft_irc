/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/01 16:16:38 by JFikents         ###   ########.fr       */
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

using std::string;

typedef std::array<pollfd, BACKLOG_SIZE + 1>	t_PollFDs;
typedef std::unordered_map<int, Client>			t_ClientMap;
typedef std::unique_ptr<Server>					t_ServerPtr;

class Client;
class Channel;

class Server
{
	private:
		static t_ServerPtr	_instance;
		static bool			_sig;
		int					_port;
		int					_socketFd;
		t_ClientMap			_clients;
		t_PollFDs			_pollFDs;
		string				_password;
		sockaddr_in			_serverAddr;

		Server(const string &port, const string &&password);

		void	_initSocket();
		void	_initPollFDs();
		void	_startMainLoop();
		void	_closeFD();

		void	acceptClient();
		void	receiveMessage(pollfd &pollFD);
		void	disconnectClient(pollfd &pollFD);
		void	sendMessage(const int &fd);
		void	parseMessage(const int &fd);
		void	executeCommand(const eCommand &command, string &line,
					const int &fd);
		void	debugBypass(string &line);
		void	Pong(const int &fd, const string &line);
		void	doCapNegotiation(const int &fd, string &line);
		void	checkConnectionTimeout(pollfd &pollFD);
		void	checkPassword(const int &fd, const string &line);
		void	joinChannel(const int &fd, string &line);
	
	public:
		const Client	&getClientByNickname(const string &nickname) const;
		static Server	&getInstance(const string & = "", const string && = "");
		static void		sigAction(int sig);

		Server()								= delete;
		Server(const Server &other)				= delete;
		Server &operator=(const Server &other)	= delete;
		~Server();
		
		void	initServer();
		void	reload();
};

#endif
