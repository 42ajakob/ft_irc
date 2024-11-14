/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientOpMethods.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 15:16:56 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/14 19:57:06 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Operator.hpp"

void	Client::giveOperatorAccess(string &&username, const string &password)
{
	try{
		if (_operatorAccess != nullptr)
			throw std::runtime_error(ERR_ALREADYREGISTRED(_operatorAccess->getUsername()));
		_operatorAccess = std::make_unique<Operator>(std::move(username), password);
		addToSendBuffer(RPL_YOUREOPER(_Nickname));
	}
	catch (std::exception &e){
		std::cerr << "Error giving operator access: " << e.what() << std::endl;
		addToSendBuffer(e.what());
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
