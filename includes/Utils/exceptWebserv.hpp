/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptWebserv.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:45:08 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:45:09 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECPTWEBSERV_HPP
# define EXECPTWEBSERV_HPP

# include <exception>
# include <string>

class exceptWebserv: public std::exception
{
	public:
		exceptWebserv(const std::string content) throw();
		const char *what() const throw();
		~exceptWebserv() throw();

	private:
		std::string _content;
};

#endif
