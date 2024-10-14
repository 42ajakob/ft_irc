/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/14 20:15:52 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Client.hpp"

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

void Client::send(std::string message) {
	send(fd, message.c_str(), message.length(), 0);
}

std::string Client::getIp() {
	return (ip);
}

std::string Client::getNick() {
	return (nick);
}

std::string Client::getRealname() {
	return (realname);
}

void Client::setNick(std::string nick_value) {
	nick = nick_value;
}

void Client::setRealname(std::string realname_value) {
	realname = realname_value;
}

void Client::setUsername(std::string username_value) {
	username = username_value;
}

std::string Client::getUsername() {
	return (username);
}

void Client::setHostname(std::string hostname_value) {
	hostname = hostname_value;
}

std::string Client::getHostname() {
	return (hostname);
}
