/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/17 17:00:55 by JFikents         ###   ########.fr       */
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
	std::string	_nickname;
	std::string	_hostName;
	std::string	_userName;

	bool	isNicknameAvailable(std::string nickname);

public:
	Client(const Client &other)				= delete;
	Client &operator=(const Client &other)	= delete;
	Client();
	~Client();

	void				setFd(int fd_value);
	void				setNickname(std::string nickname);
	void				setUserName(std::string username);
	int					getFd() const;
	const std::string	&getNickname() const;

	bool operator ==(const Client &other) const;
};

#endif
