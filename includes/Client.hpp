/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:49 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/14 20:13:41 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
private:
	int fd;
	std::string ip;
public:
	Client();
	~Client();
	Client(const Client &other);
	Client &operator=(const Client &other);
	int	getFd();
	void setFd(int fd_value);
	void setIp(std::string ip_value);
	void send(std::string message);
	std::string getIp();
	std::string getNick();
	std::string getRealname();
};

#endif
