/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:09 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:11 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Error.hpp"

std::map<int, std::string> Error::_errMap;
int Error::init = 0;

Error::Error()
{
	if(init == 0)
	{
		init = 1;
		initMap();
	}
}

Error::~Error()
{

}

void Error::initMap()
{
	_errMap[100] = "Continue";
	_errMap[101] = "Switching Protocols";
	_errMap[200] = "OK";
	_errMap[201] = "Created";
	_errMap[202] = "Accepted";
	_errMap[203] = "Non-Authoritative Information";
	_errMap[204] = "No Content";
	_errMap[205] = "Reset Content";
	_errMap[206] = "Partial Content";
	_errMap[300] = "Multiple Choices";
	_errMap[301] = "Moved Permanently";
	_errMap[302] = "Found";
	_errMap[303] = "See Other";
	_errMap[304] = "Not Modified";
	_errMap[305] = "Use Proxy";
	_errMap[306] = "(Unused)";
	_errMap[307] = "Temporary Redirect";
	_errMap[400] = "Bad Request";
	_errMap[401] = "Unauthorized";
	_errMap[402] = "Payment Required";
	_errMap[403] = "Forbidden";
	_errMap[404] = "Not Found";
	_errMap[405] = "Method Not Allowed";
	_errMap[406] = "Not Acceptable";
	_errMap[407] = "Proxy Authentication Required";
	_errMap[408] = "Request Timeout";
	_errMap[409] = "Conflict";
	_errMap[410] = "Gone";
	_errMap[411] = "Length Required";
	_errMap[412] = "Precondition Failed";
	_errMap[413] = "Request Entity Too Large";
	_errMap[414] = "Request-URI Too Long";
	_errMap[415] = "Unsupported Media Type";
	_errMap[416] = "Requested Range Not Satisfiable";
	_errMap[417] = "Expectation Failed";
	_errMap[500] = "Internal Server Error";
	_errMap[501] = "Not Implemented";
	_errMap[502] = "Bad Gateway";
	_errMap[503] = "Service Unavailable";
	_errMap[504] = "Gateway Timeout";
	_errMap[505] = "Version Not Supported";
}

std::string Error::getError(int nbr)
{
	std::map<int, std::string>::iterator itfind = _errMap.find(nbr);
	if(itfind == _errMap.end())
		throw exceptWebserv("Error : not relative error code found");
	return(itfind->second);
}