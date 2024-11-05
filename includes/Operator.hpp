/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:13:00 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/05 16:42:55 by JFikents         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATOR_HPP
# define OPERATOR_HPP

# include "Utils.hpp"
# include <string>
# include <unordered_map>
# include <unordered_set>

# ifndef CREDENTIALS_FILE
#  define CREDENTIALS_FILE ".operator_credentials"
# endif

typedef std::unordered_map<string, string>	t_OperatorCredentials;

class Operator
{
	private:
		static t_OperatorCredentials	_credentials;
		static t_StringSet				_loggedOperators;
		const string					&_username;

	public:
		Operator()									= delete;
		Operator(const Operator &other)				= delete;
		Operator &operator=(const Operator &other)	= delete;
		Operator(const string &&username, const string &password);
		~Operator();

		static void	loadCredentials();
};

#endif