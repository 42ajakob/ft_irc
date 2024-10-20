/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/20 22:24:32 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <unordered_set>

class Client
{
private:
	static std::unordered_set<std::string>	_usedNicknames;
	int										_fd = -1;
	std::string								_Nickname;
	std::string								_Username;
	std::string								_Hostname;
	std::string 							_sendBuffer;
	std::string 							_recvBuffer;
	bool									_registered = false;

	bool									isNicknameAvailable(std::string nickname);

public:
	Client(const Client &other)				= delete;
	Client &operator=(const Client &other)	= delete;
	Client();
	~Client();

	void				setFd(int fd_value);
	void				setNickname(std::string nickname);
	void				setUsername(std::string username);
	void				setHostname(std::string &Hostname);

	const int			&getFd() const;
	const std::string	&getNickname() const;
	const std::string	&setUsername() const;
	const std::string	&getIpAddr() const;
	const std::string	&getSendBuffer() const;
	const std::string	&getRecvBuffer() const;

	void				addToSendBuffer(std::string buffer);
	void				addToRecvBuffer(std::string buffer);
	void				clearSendBuffer();
	void				clearRecvBuffer();
	void				markAsRegistered();
	const bool			&IsRegistered() const;
	bool				operator==(const Client &other) const;
};

#endif
