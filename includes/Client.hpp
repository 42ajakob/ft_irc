/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 19:24:44 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Utils.hpp"
# include "Operator.hpp"
# include <iostream>
# include <unordered_set>
# include <chrono>
# include <arpa/inet.h>
# include <memory>
# include "numericReplies.hpp"

class Operator;

typedef std::unique_ptr<Operator>	t_OperatorAccess;

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

		t_OperatorAccess	_operatorAccess = nullptr;

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
		void				addToSendBuffer(string buffer) noexcept;
		void				addToRecvBuffer(string buffer) noexcept;
		void				clearSendBuffer();
		void				clearRecvBuffer();
		const string		&getSendBuffer() const;
		const string		&getRecvBuffer() const;
		void				sendPrivMsg(const string &msg, const string &origin) noexcept;

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

	// *** Operator methods ***
		void				giveOperatorAccess(string &&username, const string &password);
		void				revokeOperatorAccess();
		bool				isOperator() const;
		void				addOperator(const string &username, const string &password);
		void				removeOperator(const string &username);
		void				listOperators() noexcept;
};

#endif
