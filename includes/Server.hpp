/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/14 18:42:53 by ajakob           ###   ########.fr       */
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

		typedef void (Server::*t_Command)(Client &, const std::string &);

	// *** Initialization and Closing methods ***
		void	_initSocket();
		void	_initPollFDs();
		void	_startLoop();
		void	_closeFD();

	// *** I/O loop Methods ***
		void	_acceptClient();
		void	_receiveMessage(pollfd &pollFD);
		void	_disconnectClient(pollfd &pollFD);
		void	_sendClientBuffer(const int &fd);
		void	_parseMessage(const int &fd);
		void	_executeCommand(const eCommand &command, string &line,
					const int &fd);

	// *** Connection methods ***
		void	_handleCap		(Client &client, const string &line);
		void	_handlePass		(Client &client, const string &line);
		void	_handlePing		(Client &client, const string &line);
		void	_handlePong		(Client &client, const string &line);
		void	_handleNick		(Client &client, const string &line);
		void	_handleUser		(Client &client, const string &line);
		void	_checkConnectionTimeout(pollfd &pollFD);


	// *** Operator methods ***
		void	_Oper			(Client &client, const string &line);
		void	_addOper		(Client &client, const string &line);
		void	_rmOper			(Client &client, const string &line);
		void	_lsOper			(Client &client, const string &line);

	// *** Command methods ***
		void	_handleJoin		(Client &client, const string &line);
		void 	_handleInvite	(Client &client, const string &line);
		void	_handleTopic	(Client &client, const string &line);
		void	_handleQuit		(Client &client, const string &line);
		void	_handleKick		(Client &client, const string &line);
		void	_handlePrivMsg	(Client &client, const string &line) noexcept;
		void	_handlePart		(Client &client, const string &line);
		void	_handleWho		(Client &client, const string &line);
		void	_modeLoop		(Client &client, Channel &channel, const string &mode, string &mode_params);
		void	_handleMode		(Client &client, string const &line);

	// *** Debug and error methods ***
		void	_OpBypass	(Client &client, const string &line);
		void	_logError	(Client &client, const string &error) const;
	
	public:
		Client			&getClientByNickname(const string &nickname);
		static Server	&getInstance(const string & = "", const string && = "");
		static void		sigAction(int sig);

		Server()								= delete;
		Server(const Server &other)				= delete;
		Server &operator=(const Server &other)	= delete;
		~Server();
		
		void	initServer();
		void	reload();
		void	setTimestamp();
		string	getTimestamp();
};

#endif
