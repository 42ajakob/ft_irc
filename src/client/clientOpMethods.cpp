/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientOpMethods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:16:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/06 19:56:05 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Operator.hpp"

void	Client::giveOperatorAccess(string &&username, const string &password)
{
	try{
		_operatorAccess = std::make_unique<Operator>(std::move(username), password);
		addToSendBuffer(":FT_IRC 381 " + _Nickname + " :You are now a Server operator\r\n");
	}
	catch (std::exception &e){
		std::cerr << "Error giving operator access: " << e.what() << std::endl;
		string	errorType = e.what();
		if (errorType == "Wrong operator credentials")
			addToSendBuffer(":FT_IRC 464 " + _Nickname + " :Wrong Credentials\r\n");
		else if (errorType == "Operator already logged in")
			addToSendBuffer(":FT_IRC 462 " + _Nickname + " :You may not re-register\r\n");
		else
			addToSendBuffer(":FT_IRC 464 " + _Nickname + " :No Serv-Operators are registered\r\n");
	}
}

void	Client::revokeOperatorAccess()
{
	_operatorAccess.reset();
}

bool	Client::isOperator() const
{
	return (_operatorAccess != nullptr);
}

void	Client::addOperator(const string &username,const string &password)
{
	if (_operatorAccess == nullptr)
	{
		addToSendBuffer(":FT_IRC 481 " + _Nickname + " :Permission Denied- You're not an IRC operator\r\n");
		return ;
	}
	try{
		this->_operatorAccess->addOperator(username, password);
	}
	catch (std::exception &e){
		std::cerr << "Error adding operator: " << e.what() << std::endl;
		addToSendBuffer(":FT_IRC ERROR " + _Nickname + " " + e.what() + "\r\n");
	}
}

void	Client::removeOperator(const string &username)
{
	if (_operatorAccess == nullptr)
	{
		addToSendBuffer(":FT_IRC 481 " + _Nickname + " :Permission Denied- You're not an IRC operator\r\n");
		return ;
	}
	try{
		this->_operatorAccess->removeOperator(username);
	}
	catch (std::exception &e){
		std::cerr << "Error removing operator: " << e.what() << std::endl;
		addToSendBuffer(":FT_IRC ERROR " + _Nickname + " " + e.what() + "\r\n");
	}
}
