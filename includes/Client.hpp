/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/18 19:33:52 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <vector>

class Client
{
private:
	static std::vector<std::string>	_usedNicknames;
	int			_fd;
	std::string	_nickName;
	std::string	_ipAddr;
	std::string	_userName;
	std::string _lastMsg;

	bool	isNicknameAvailable(std::string nickname);

public:
	Client(const Client &other)				= delete;
	Client &operator=(const Client &other)	= delete;
	Client();
	~Client();

	void				setFd(int fd_value);
	void				setNickName(std::string nickname);
	void				setIpAddr(std::string ipAddr);
	void				setUserName(std::string username);
	void				setLastMsg(std::string buffer);

	int					getFd() const;
	const std::string	&getNickname() const;
	const std::string	&getIpAddr() const;
	const std::string	&getLastMsg() const;

	bool operator==(const Client &other) const;
};

#endif
