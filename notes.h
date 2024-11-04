/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 19:18:23 by JFikents          #+#    #+#             */
/*   Updated: 2024/10/20 15:57:53 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//@ ----------------------------- Server Notes ----------------------------- @//
/*
	To simplify the implementation (and comply with the IRC Protocol), we will
not consider the following:
	@Server class constraints:
	- The Backlog size can be tuned to the needs of the server, 512 is a
		common value for a medium-sized server.
	- The server needs to be created with a password and a port number.
	- Will keep track of the clients connected to the server. It will store
		the file descriptor of the client and the Client object in a map.
	- Will keep track of the Channels created.
	- All messages sent and received need to be always terminated with a
		'\r\n' sequence, and the maximum length of a message is 512 characters,
		including the '\r\n'.
	- Each message sent to the server will be in the format:
		":<prefix> <command> <params (max 15)> :<trailing>\r\n"
		or
		"<command> <params (max 15)> :<trailing>\r\n"
		? Notice that when the prefix is present, the first character of the
		?	parameter is a colon ':', and all the parameters are separated
		?	by a space ' ', except for the last one (trailing), which is
		?	separated by a ':' and can contain spaces.
	@Server Functionality:
				--- this ---
	- The server will ask for a password when a new Client connects.
	- The server will require the client to send a NICK and a USER command
		before joining any channel.
				--- or ---
	- After the client joins the server, It will need to send a NICK, a USER,
		and a PASS command before doing anything else.
				--- end ---
	- When joining a channel, the server will try to find the channel, if it
		doesn't exist, it will create it, and the client will be the operator.
	- The server will be in a loop of constant polling, checking for new
		connections, and messages from the clients and its own socket.
*/
//@ ---------------------------- Channel Notes ---------------------------- @//
/*
	To simplify the implementation (and comply with the IRC Protocol), we will
not consider the following:
	@Channel class constraints:
	- Will keep track of the names used to avoid duplicates.
	- Can't be copied, or assigned.
	- Keep track of the members of the channel.
	- Keep track of the operators of the channel.
	- The topic must be set when the channel is created.
	- The topic can be changed by anyone unless the channel mode is set to
		protected-topic.
	- Can have a password to join the channel. It will take effect if the
		channel mode is set to password-protected.
	- Can have a user limit. It will take effect if the channel mode is set to
		user-limit.
	@Name constraints:
	- The name should be unique.
	- The name should start with a '&' or '#' character.
	- The name should not contain any spaces, control G (^G), commas,
		or colons ':'.
	- Names are case-insensitive, therefore must be stored in lowercase
		only.
	- The maximum length of 50 characters.
	@Channel Functionality:
	- The channel will allow clients to send messages to everyone in the
		channel.
	- The channel will be controlled by the operators.
	- The channel has the following modes:
		- Invite-only: Only invited clients can join the channel.
		- Protected-topic: Only operators can change the topic.
		- Password-protected: Clients need to send a PASS command with the
			password before joining the channel.
		- User-limit: The channel has a limit of users that can join.
	- When a client joins a channel, it will receive the topic of the channel.
	- When the last client leaves the channel, the channel will be destroyed.
	- When the last operator leaves the channel, the channel will assign
		operator privileges to another client (the how is TBD).
*/
//@ ----------------------------- Client Notes ----------------------------- @//
/*
	To simplify the implementation (and comply with the IRC Protocol), we will
not consider the following:
	@Client class constraints:
	- The client will have a file descriptor, a nickname, a
		hostname, and a username.
	- Can't be copied, or assigned.
	- The username and nickname will be set by the client when connecting to
		the server.
	- The username doesn't need to be unique.
	- The hostname will be set by the server when the client connects.
	- The nickname can be changed by the client at any time.
	- Nicknames can't be "anonymous", and must be unique with a maximum
	length of 9 characters, truncated if longer.
	- Nicknames can't contain spaces, and must start with an alphanumeric
	character.
	- Nicknames are case-insensitive, therefore must be stored in lowercase
	only.
	@Client Functionality:
	- The client will be able to send messages to the server.
	- The client will be able to join channels.
	- The client will be able to create channels when they don't exist.
	- The client will be able to leave channels.
	- The client will be able to change its nickname. Not required by the subject.
	- The client will be able to change its username. Maybe?
	- The client will be able to change the topic of a channel if it is not
		protected or if it is the operator of the channel.
	- If the client is an operator of a channel, it will be able to kick
		other clients from the channel.
	- If the client is an operator of a channel, it will be able to invite
		other clients to the channel.
	- If the client is an operator of a channel, it will be able to change
		the channel mode.
*/