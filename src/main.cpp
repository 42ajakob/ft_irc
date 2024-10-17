/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:15:50 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/17 15:48:09 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"
#include "Server.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}
	Server server(argv[1], argv[2]);

	try {
		// start signals
		// SIGINT
		// SIGQUIT
		server.init();
		std::cout << "Starting server" << std::endl;
		server.start();
	}
	catch (std::exception &e) {
		server.fdCloser();
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
