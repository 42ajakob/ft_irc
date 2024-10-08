/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:15:50 by apeposhi          #+#    #+#             */
/*   Updated: 2024/10/07 17:50:54 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"
#include "Server.hpp"

int main()
{
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
