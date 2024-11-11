/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numericReplies.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:16:12 by ajakob            #+#    #+#             */
/*   Updated: 2024/11/10 19:22:07 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define RPL_WELCOME(client, nick, user, host) (":FT_IRC 001 " + client + " :Welcome to the Internet Relay Network "  + nick + "!" + user + "@" + host + "\r\n")
#define RPL_YOURHOST(client) (":FT_IRC 002 " + client + " :Your host is ft_irc, running version 0.0.1\r\n")
#define RPL_CREATED(client, date) (":FT_IRC 003 " + client + " :This server was created " + date + "\r\n")
#define RPL_MYINFO(client, avail_user_modes, avail_channel_modes) (":FT_IRC 004 " + client + " ft_irc 0.0.1 " + avail_user_modes + " " + avail_channel_modes + "\r\n")

#define RPL_UMODEIS(user_mode) (":FT_IRC 221 " + user_mode + "\r\n")

#define RPL_CHANNELMODEIS(channel, mode, mode_params) (":FT_IRC 324 " + channel + " " + mode + " " + mode_params + "\r\n")
#define RPL_UNIQOPIS(channel, nick) (":FT_IRC 325 " + channel + " " + nick + "\r\n")

#define RPL_NOTOPIC(client, channel) (":FT_IRC 331 " + client + " " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(client, channel, topic) (":FT_IRC 332 " + client + " " + channel + " :" + topic + "\r\n")

#define RPL_INVITING(channel, nick) (":FT_IRC 341 " + channel + " " + nick + "\r\n")
#define RPL_INVITELIST(channel, invite_mask) (":FT_IRC 346 " + channel + " " + invite_mask + "\r\n")
#define RPL_ENDOFINVITELIST(channel) (":FT_IRC 347 " + channel + " :End of channel invite list\r\n")

#define RPL_NAMREPLY(client, channel, member_list) (":FT_IRC 353 " + client + " = " + channel + " :" + member_list + "\r\n")
#define RPL_ENDOFNAMES(client, channel) (":FT_IRC 366 " + client + " " + channel + " :End of /NAMES list\r\n")

#define RPL_YOUREOPER(client) (":FT_IRC 381 " + client + " :You are now an IRC operator\r\n")

#define ERR_NOSUCHNICK(nick) (":FT_IRC 401 " + nick + " :No such nick\r\n")
#define ERR_NOSUCHSERVER() (":FT_IRC 402 :No such server\r\n")
#define ERR_NOSUCHCHANNEL(channel) (":FT_IRC 403 " + channel + " :No such channel\r\n")
#define ERR_CANNOTSENDTOCHAN(channel) (":FT_IRC 404 " + channel + " :Cannot send to channel\r\n")
#define ERR_TOOMANYTARGETS(targets) (":FT_IRC 407 " + targets + " :Max of 4 targets allowed per PRIVMSG\r\n")
#define ERR_NOORIGIN() (":FT_IRC 409 :No origin specified\r\n")

#define ERR_NORECIPIENT(command) (":FT_IRC 411 :No recipient given (" + command + ")\r\n")
#define ERR_NOTEXTTOSEND() (":FT_IRC 412 :No text to send\r\n")

#define ERR_NONICKNAMEGIVEN(client) (":FT_IRC 431 " + client + " :No nickname given\r\n")
#define ERR_ERRONEUSNICKNAME(client, nick) (":FT_IRC 432 " + client + " " + nick + " :Erroneus nickname\r\n")
#define ERR_NICKNAMEINUSE(client, nick) (":FT_IRC 433 " + client + " " + nick + " :Nickname is already in use\r\n")

#define ERR_USERNOTINCHANNEL(nick, channel) (":FT_IRC 441 " + nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channel) (":FT_IRC 442 " + channel + " :You're not on that channel\r\n")
#define ERR_USERONCHANNEL(client, nick, channel) (":FT_IRC 443 " + client + " " + nick + " " + channel + " :is already on channel\r\n")

#define ERR_NOTREGISTERED(client) (":FT_IRC 451 " + client + " :You have not registered\r\n")

#define ERR_NEEDMOREPARAMS(client, command) (":FT_IRC 461 " + client + " " + command + " :Not enough parameters\r\n")
#define ERR_ALREADYREGISTRED(client) (":FT_IRC 462 " + client + " :You may not reregister\r\n")
#define ERR_PASSWDMISMATCH(client) (":FT_IRC 464 " + client + " :Password incorrect\r\n")
#define ERR_KEYSET(channel) (":FT_IRC 467 " + channel + " :Channel key already set\r\n")

#define ERR_CHANNELISFULL(client, channel) (":FT_IRC 471 " + client + " " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_UNKNOWNMODE(char, channel) (":FT_IRC 472 " + char + " :is unknown mode char to me for " + channel + "\r\n")
#define ERR_INVITEONLYCHAN(client, channel) (":FT_IRC 473 " + client + " " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(client, channel) (":FT_IRC 475 " + client + " " + channel + " :Cannot join channel (+k)\r\n")
#define ERR_BADCHANMASK(channel) (":FT_IRC 476 " + channel + " :Bad Channel Mask\r\n")

#define ERR_NOPRIVILEGES() (":FT_IRC 481 :Permission Denied- You're not an IRC operator\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) (":FT_IRC 482 " + channel + " :You're not channel operator\r\n")

#define ERR_NOOPERHOST() (":FT_IRC 491 :No O-lines for your host\r\n")

#define ERR_UMODEUNKNOWNFLAG(client) (":FT_IRC 501 " + client + " :Unknown MODE flag\r\n")
#define ERR_USERSDONTMATCH(client) (":FT_IRC 502 " + client + " :Cant change mode for other users\r\n")