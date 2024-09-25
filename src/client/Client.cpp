/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/23 15:19:38 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{}

Client::~Client()
{}

Client::Client(const Client &other)
{
	(void)other;
}

Client &Client::operator=(const Client &other)
{
	(void)other;
	return (*this);
}

int Client::getFd() {
	return (fd);
}

void Client::setFd(int fd_value) {
	fd = fd_value;
}

void Client::setIp(std::string ip_value) {
	ip = ip_value;
}


