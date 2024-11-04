/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/01 17:24:13 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unordered_set>
# include <chrono>
# include "Utils.hpp"
#include <arpa/inet.h>

using std::string;

typedef std::unordered_set<string>	t_StringSet;

class Client
{
	private:
		static t_StringSet	_usedNicknames;
		string				_Nickname;
		string				_Username;
		string				_Hostname;
		string				_sendBuffer;
		string				_recvBuffer;
		bool				_registered = false;
		t_TimeStamp			_programmedDisconnection;
		bool				_isPingSent = true;
		bool				_isPasswordCorrect = false;

		bool				_isNicknameAvailable(string nickname);
		void				_markAsRegistered();

	public:
		Client(const Client &other)				= delete;
		Client &operator=(const Client &other)	= delete;
		Client();
		~Client();

		const string		&getNickname() const;
		const string		&getUsername() const;
		const string		&getHostname() const;

		bool				operator==(const Client &other) const;
		bool				operator==(const string &nickname) const;

	// *** Buffer methods ***
		void				addToSendBuffer(string buffer);
		void				addToRecvBuffer(string buffer);
		void				clearSendBuffer();
		void				clearRecvBuffer();
		const string		&getSendBuffer() const;
		const string		&getRecvBuffer() const;

	// *** Registration methods ***
		const bool			&IsPasswordCorrect() const;
		const bool			&IsRegistered() const;
		void				setHostname(const sockaddr_in &clientAddr);
		void				setNickname(string &&nickname);
		void				setPasswordCorrect(const bool);
		void				setUsername(string &&username);

	// *** Timeout methods ***
		const t_TimeStamp	&getProgrammedDisconnection() const;
		void				pingClient();
		void				resetPingTimerIfPongMatches(const string &line);
		void				setProgrammedDisconnection(const int seconds, bool setByQuitCommand = false);
};

#endif
