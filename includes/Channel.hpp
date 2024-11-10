/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajakob <ajakob@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 21:45:05 by apeposhi          #+#    #+#             */
/*   Updated: 2024/11/10 14:41:57 by ajakob           ###   ########.fr       */
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
typedef	std::unordered_set<const Client*>	t_ClientSet;

class	Channel
{
	private:
		typedef struct ChannelCreatorKey {} t_ChannelCreatorKey;

	public:
		Channel()									= delete;
		Channel(const Channel &other)				= delete;
		Channel	&operator=(const Channel &other)	= delete;
		Channel(const t_ChannelCreatorKey &key, const string &name,
			const Client &creator);
		~Channel();

		void	join(Client &client, const string &password);
		void	sendChannelInfo(Client &client);
		void	kick(const string &nickname, const Client &client);
		void	invite(const string &nickname, Client &client);
		void	leave(const Client &client);
		void	mode(string mode,Client &client);
		void	clear();
		void	printMembers() const;

		bool	operator==(const Channel &other) const;
		bool	operator==(const string &name) const;

		static Channel	&getChannel(const string &name, const Client &client);
		static Channel	&getChannel(const string &name);
		static void		clientDisconnected(const Client &client);

	enum class Mode
	{
		InviteOnly,
		ProtectedTopic,
		PasswordProtected,
		UserLimit,
		Count
	};

	private:
		static constexpr size_t	ModeCount = static_cast<size_t>(Mode::Count);
		static t_ChannelMap		_channels;

		Channel(const string &name, const Client &creator);

		bitset<ModeCount>	_mode;
		string				_name;
		t_ClientSet			_members;
		t_ClientSet			_operators;
		t_ClientSet			_invited;
		string				_topic;
		string				_password;
		uint16_t			_userLimit;
};


#endif
