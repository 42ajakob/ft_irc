/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/20 20:39:39 by ajakob           ###   ########.fr       */
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
	std::string								_ipAddr;
	std::string 							_sendbuffer;
	std::string 							_recvBuffer;

	bool									isNicknameAvailable(std::string nickname);

public:
	Client(const Client &other)				= delete;
	Client &operator=(const Client &other)	= delete;
	Client();
	~Client();

	void				setFd(int fd_value);
	void				setNickname(std::string nickname);
	void				setUsername(std::string username);
	void				setIpAddr(std::string ipAddr);
	void				setSendBuffer(std::string buffer);
	void				setRecvBuffer(std::string buffer);

	const int			&getFd() const;
	const std::string	&getNickname() const;
	const std::string	&setUsername() const;
	const std::string	&getIpAddr() const;
	const std::string	&getSendBuffer() const;
	const std::string	&getRecvBuffer() const;

	void				clearSendBuffer();
	void				clearRecvBuffer();
	bool				operator==(const Client &other) const;
};

#endif
