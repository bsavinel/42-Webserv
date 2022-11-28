/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headerRespond.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:19 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:20 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

std::string HeaderRespond (off_t contentLenght, int statusCode, std::string type = std::string(), std::vector<std::string> cookies = std::vector<std::string>())
{
	std::stringstream ss;
	std::string	header;
	Error err;

	ss << statusCode;
	header += "HTTP/1.1 " + ss.str() + " " + err.getError(statusCode) + "\n";
	if (type.size() > 0)
		header += "Content-Type: " + type + "\n";
	if (contentLenght > 0)
	{
		ss.str("");
		ss << contentLenght;
		header += "Content-Length: " + ss.str() + "\n";
	}
	if (!cookies.empty())
	{
		for (std::vector<std::string>::iterator it = cookies.begin(); it != cookies.end(); it++)
			header += *it + "\r\n";
	}
	header += "\n";
	return header;
}