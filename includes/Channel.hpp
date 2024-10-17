/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:45:05 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/17 15:06:23 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "Client.hpp"
# include <vector>
# include <bitset>

class Client;
class Server;

class Channel
{
	public:
		Channel()									= delete;
		Channel(const Channel &other)				= delete;
		Channel &operator=(const Channel &other)	= delete;
		Channel(std::string name, const Client &creator, const Server &server);
		~Channel();

		void join(Client &client);
		void kick(const std::string &nickname);
		void invite(const std::string &nickname);
		void leave(Client &client);
		void mode(Client &client);
		void clear();

	enum class Mode
	{
		InviteOnly,
		ProtectedTopic,
		PasswordProtected,
		UserLimit,
		Count
	};

	private:
		static std::vector<std::string>	_usedNames;

		std::bitset<static_cast<size_t>(Mode::Count)>	_mode;
		std::string					_name;
		std::vector<const Client*>	_members;
		std::vector<const Client*>	_operators;
		std::string					_topic;
		std::string					_password;
		uint16_t					_userLimit;
		const Server				*_server;

		bool	isNameAvailable(std::string name);
};


#endif
