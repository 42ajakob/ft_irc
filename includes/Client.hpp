/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/22 19:56:35 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unordered_set>
# include <chrono>
# include "Utils.hpp"

using std::string;

typedef std::unordered_set<string>	t_StringSet;

class Client
{
private:
	static t_StringSet	_usedNicknames;
	int					_fd = -1;
	string				_Nickname;
	string				_Username;
	string				_Hostname;
	string				_sendBuffer;
	string				_recvBuffer;
	bool				_registered = false;
	t_TimeStamp			_connectionTime;
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

	void				setFd(int fd_value);
	void				setNickname(string nickname);
	void				setUsername(string username);
	void				setHostname(string &&Hostname);

	const int			&getFd() const;
	const string		&getNickname() const;
	const string		&getUsername() const;
	const string		&getHostname() const;
	bool				operator==(const Client &other) const;

// *** Buffer methods ***
	const string		&getSendBuffer() const;
	const string		&getRecvBuffer() const;
	void				addToSendBuffer(string buffer);
	void				addToRecvBuffer(string buffer);
	void				clearSendBuffer();
	void				clearRecvBuffer();

// *** Registration methods ***
	const t_TimeStamp	&getConnectionTime() const;
	void				setPasswordCorrect(const bool);
	const bool			&IsPasswordCorrect() const;
	const bool			&IsRegistered() const;

// *** Timeout methods ***
	void				pingClient();
	void				resetPingTimer(const string &line);
	void				setProgrammedDisconnection(std::chrono::seconds seconds);
	const t_TimeStamp	&getProgrammedDisconnection() const;
};

#endif
