/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:45:18 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:45:19 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_HPP
#define ERROR_HPP

# include <string>
# include <map>
# include <fcntl.h>
# include "exceptWebserv.hpp"

std::string buildErrorPage(int err);

class Error
{
	public :
		typedef std::map<int, std::string> ErrMap;

		Error();
		~Error();
		
		std::string getError(int nbr);
		
	private :
		static ErrMap _errMap;
		static int init;
		static void initMap();
		
};

#endif