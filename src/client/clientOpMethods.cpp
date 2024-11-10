/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientOpMethods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:16:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/09 13:51:21 by ajakob           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Operator.hpp"

void	Client::giveOperatorAccess(string &&username, const string &password)
{
	try{
		_operatorAccess = std::make_unique<Operator>(std::move(username), password);
		addToSendBuffer(RPL_YOUREOPER(_Nickname));
	}
	catch (std::exception &e){
		std::cerr << "Error giving operator access: " << e.what() << std::endl;
		string	errorType = e.what();
		if (errorType == "Wrong operator credentials")
			addToSendBuffer(ERR_PASSWDMISMATCH(_Nickname));
		else if (errorType == "Operator already logged in")
			addToSendBuffer(ERR_ALREADYREGISTRED(_Nickname));
		else
			addToSendBuffer(ERR_NOOPERHOST());
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
		addToSendBuffer(ERR_NOPRIVILEGES());
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
		addToSendBuffer(ERR_NOPRIVILEGES());
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

void	Client::listOperators() noexcept
{
	if (_operatorAccess == nullptr)
	{
		addToSendBuffer(ERR_NOPRIVILEGES());
		return ;
	}
	const string &ops = this->_operatorAccess->listOperators();
	addToSendBuffer(":FT_IRC " + _Nickname + " :Operators: " + ops + "\r\n");
}
