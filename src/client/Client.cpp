/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apeposhi <apeposhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:04:41 by apeposhi          #+#    #+#             */
/*   Updated: 2024/09/12 17:15:28 by apeposhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{}

Client::~Client()
{}

Client::Client(const Client &other)
{
	(void)other;
}

Client &Client::operator=(const Client &other)
{
	(void)other;
	return (*this);
}
