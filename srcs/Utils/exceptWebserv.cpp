/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptWebserv.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:12 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:14 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exceptWebserv.hpp"

exceptWebserv::exceptWebserv(const std::string content) throw()
{
	this->_content = content;
}

exceptWebserv::~exceptWebserv() throw()
{
	
}

const char *exceptWebserv::what() const throw()
{
	return this->_content.c_str();
}