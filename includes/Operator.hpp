/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JFikents <Jfikents@student.42Heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 19:13:00 by JFikents          #+#    #+#             */
/*   Updated: 2024/11/07 15:12:34 by JFikents         ###   ########.fr       */
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
		const string					_username;

	public:
		Operator()									= delete;
		Operator(const Operator &other)				= delete;
		Operator &operator=(const Operator &other)	= delete;
		Operator(string &&username, const string &password);
		~Operator();

		static void		loadCredentials();
		void			addOperator(const string &username, const string &pass);
		void			removeOperator(const string &username);
		const string	&getUsername() const noexcept;
		const string	listOperators() const noexcept;
};

#endif