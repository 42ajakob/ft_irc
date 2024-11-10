/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 15:26:11 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include "Operator.hpp"
# include "Utils.hpp"
# include <iostream>
# include <vector>
# include <unordered_map>
# include <sys/socket.h>
# include <sys/types.h> // getaddrinfo, bind, connect
# include <arpa/inet.h> // htons, htonl, ntohl, ntohs, inet_addr, inet_ntoa
# include <fcntl.h>
# include <poll.h>
# include <unistd.h>
# include <array>
# include <memory>

# define BACKLOG_SIZE 512

class Client;
class Channel;

typedef std::array<pollfd, BACKLOG_SIZE + 1>	t_PollFDs;
typedef std::unordered_map<int, Client>			t_ClientMap;
typedef std::unique_ptr<Server>					t_ServerPtr;

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
		string				_timestamp;

		Server(const string &port, const string &&password);

		void	_initSocket();
		void	_initPollFDs();
		void	_startMainLoop();
		void	_closeFD();

		void	_acceptClient();
		void	_receiveMessage(pollfd &pollFD);
		void	_disconnectClient(pollfd &pollFD);
		void	_sendMessage(const int &fd);
		void	_parseMessage(const int &fd);
		void	_executeCommand(const eCommand &command, string &line,
					const int &fd);
		void	_debugBypass(string &line);
		void	_Pong(const int &fd, const string &line);
		void	_doCapNegotiation(const int &fd, string &line);
		void	_checkConnectionTimeout(pollfd &pollFD);
		void	_checkPassword(const int &fd, const string &line);
		void	_joinChannel(const int &fd, string &line);
		void 	_invite(Client &client, const string &line);
		void	_quitClient(const int &fd);
		void	_Oper(const int &fd, string &line);
		void	_addOper(const int &fd, string &line);
		void	_rmOper(const int &fd, string &line);

		void	_parse_kick(Client &client, std::string const &line);
		void	_parse_mode(Client &client, std::string const &line);

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
		void	setTimestamp();
		std::string	getTimestamp();
};

#endif
