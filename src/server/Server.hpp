/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:05:36 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/12 17:15:02 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

class Server
{
private:
	/* data */
public:
	Server();
	~Server();
	Server(const Server &other);
	Server &operator=(const Server &other);
};

#endif
