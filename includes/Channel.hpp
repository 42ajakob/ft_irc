/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:45:05 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/08 21:37:48 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "Client.hpp"
# include <vector>
# include <bitset>

class Client;

class Channel
{
	private:
		static std::vector<std::string>	_usedNames;
		std::string			_name;
		std::vector<Client>	_members;
		std::vector<Client>	_operators;
		std::string			_topic;
		std::bitset<4>		_mode;
		std::string			_password;
		uint16_t			_userLimit;
		
		bool	isNameAvailable(std::string name);
	public:
		Channel()									= delete;
		Channel(const Channel &other)				= delete;
		Channel &operator=(const Channel &other)	= delete;
		Channel(std::string name,const Client &creator);
		~Channel();

		enum class Mode;

		void join(Client &client);
		void kick(const std::string &nickname);
		void invite(const std::string &nickname);
		void leave(Client &client);
		void mode(Client &client);
		void clear();
};

enum class Channel::Mode
{
	InviteOnly,
	ProtectedTopic,
	PasswordProtected,
	UserLimit
};

#endif
