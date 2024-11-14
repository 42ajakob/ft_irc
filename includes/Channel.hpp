/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:45:05 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/14 17:43:16 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <bitset>
# include <unordered_map>
# include <unordered_set>
# include "Client.hpp"

class	Client;
class	Server;
class	Channel;

using	std::string;
using	std::bitset;

typedef	std::unordered_map<string, Channel>	t_ChannelMap;
typedef	std::unordered_set<Client *>	t_ClientSet;

class	Channel
{
	private:
		enum Mode
		{
			InviteOnly,
			ProtectedTopic,
			PasswordProtected,
			Operators,
			UserLimit,
			Count
		};

		typedef struct ChannelCreatorKey {} t_ChannelCreatorKey;
		static constexpr size_t	ModeCount = static_cast<size_t>(Mode::Count);
		static t_ChannelMap		_channels;

		bitset<ModeCount>	_mode;
		string				_name;
		t_ClientSet			_members;
		t_ClientSet			_operators;
		t_ClientSet			_invited;
		string				_topic;
		string				_password;
		uint16_t			_userLimit;

		Channel(const string &name, Client &creator);

		void	_sendChannelInfo(Client &client);
		string	_getMembersList() const noexcept;
		void	_relayMsg(const string &msg, const Client *client) const noexcept;
		void	_promoteClientToOperator(const string &origin, Client &client);
		void	_demoteClientFromOperator(const string &origin, Client &client);

	public:
		Channel()									= delete;
		Channel(const Channel &other)				= delete;
		Channel	&operator=(const Channel &other)	= delete;
		Channel(const t_ChannelCreatorKey &key, const string &name,
			Client &creator);
		~Channel();

		std::ostream	&insertInfoToOutstream(std::ostream &os) const;

		void			join(Client &client, const string &password);
		void			kick(const string &nick, Client &client, string& reason);
		void			invite(const string &nickname, Client &client);
		void			topic(string &topic, Client &client);
		void			PrivMsg(const string &msg, const Client &origin) const noexcept;
		void			part(Client &client, const string &reason);
		void			whoReply(Client &client) const;
		void			mode(const string &mode, Client &client, const string &mode_param);
		void			mode(const string &mode, Client &client);

		bool			operator==(const Channel &other) const;
		bool			operator==(const string &name) const;

		static Channel	&getChannel(string &name, Client &client);
		static Channel	&getChannel(string &name);
		static void		clientDisconnected(Client &client);
};

std::ostream	&operator<<(std::ostream &os, const Channel &channel);


#endif
