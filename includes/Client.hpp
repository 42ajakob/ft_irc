/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/21 20:30:24 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unordered_set>
# include <chrono>
# include "Utils.hpp"

typedef std::unordered_set<std::string>	t_StringSet;

class Client
{
private:
	static t_StringSet	_usedNicknames;
	int					_fd = -1;
	std::string			_Nickname;
	std::string			_Username;
	std::string			_Hostname;
	std::string 		_sendBuffer;
	std::string 		_recvBuffer;
	bool				_registered = false;
	t_TimeStamp			_connectionTime;
	t_TimeStamp			_programmedDisconnection;
	bool				_isPingSent = false;
	bool				_isPasswordCorrect = false;

	bool				isNicknameAvailable(std::string nickname);
	void				_markAsRegistered();

public:
	Client(const Client &other)				= delete;
	Client &operator=(const Client &other)	= delete;
	Client();
	~Client();

	void				setFd(int fd_value);
	void				setNickname(std::string nickname);
	void				setUsername(std::string username);
	void				setHostname(std::string &&Hostname);
	void				setProgrammedDisconnection(std::chrono::seconds seconds);

	const int			&getFd() const;
	const std::string	&getNickname() const;
	const std::string	&getUsername() const;
	const std::string	&getIpAddr() const;
	const t_TimeStamp	&getConnectionTime() const;
	const t_TimeStamp	&getProgrammedDisconnection() const;

// *** Buffer methods ***
	const std::string	&getSendBuffer() const;
	const std::string	&getRecvBuffer() const;
	void				addToSendBuffer(std::string buffer);
	void				addToRecvBuffer(std::string buffer);
	void				clearSendBuffer();
	void				clearRecvBuffer();

// *** Registration methods ***
	void				setPasswordCorrect(const bool);
	const bool			&IsPasswordCorrect() const;
	const bool			&IsRegistered() const;
	void				pingClient();
	void				resetPingTimer();
	bool				operator==(const Client &other) const;
};

#endif
