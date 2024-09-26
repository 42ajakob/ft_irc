/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:15:50 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/26 20:12:13 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "client/Client.hpp"
#include "server/Server.hpp"

int main()
{
	Client client;
	Server server;

	try {
		// start signals
		// SIGINT
		// SIGQUIT
		server.init();
	}
	catch (std::exception &e) {
		server.fdCloser();
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
