/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_errorResp.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:44 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 09:11:52 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"
#include "Error.hpp"
#include <sys/stat.h>

std::string	HttpManager::initDefaultErrorResponse()
{
	std::string errResp;

	if (_errorCode == 0)
		return errResp;
	else if (_errorCode == 204)
		errResp = "HTTP/1.1 204 No Content\n\n";
	else
	{
		errResp = buildErrorPage(_errorCode);
		errResp.insert(0, HeaderRespond(errResp.size(), _errorCode, "text/html"));
	}
	return errResp;
}

bool	HttpManager::init_error_file(const std::string &error_page, std::string &errResp, int error_code)
{
	struct stat status;

	_file_fd= open(error_page.c_str(), O_RDONLY);
	stat(error_page.c_str(), &status);
	if (_file_fd< 0 || !S_ISREG(status.st_mode))
	{
		if (_file_fd>= 0)
			close(_file_fd);
		if (_errorCode == 0)
			return false;
		_isEnd = true;
		errResp = initDefaultErrorResponse();
		return false;
	}
	std::string type_file = determinateType(error_page);
	if (type_file.empty())
	{
		errResp = initDefaultErrorResponse();
		_isEnd = true;
		return false;
	}
	errResp = HeaderRespond(status.st_size, error_code, type_file);
	return true;
}

std::string	HttpManager::ErrorRespond(const Server &server)
{
	std::string errResp;
	Error err;
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	canWrite();
	if (server.getErrorMap().find(_errorCode) != server.getErrorMap().end())
	{
		if (_file_fd== -1)
            if (!init_error_file((*server.getErrorMap().find(_errorCode)).second, errResp, _errorCode))
				return errResp;
		nb_char = read(_file_fd, buffer, LEN_TO_READ);
		if (nb_char > 0)
			errResp.insert(errResp.size(), &buffer[0], nb_char);
		if (nb_char == -1)
		{
			errResp.clear();
			errResp = initDefaultErrorResponse();
		}
		if (nb_char < LEN_TO_READ)
		{
			close(_file_fd);
			_isEnd = true;
		}
	}
	else
	{
		errResp = initDefaultErrorResponse();
		_isEnd = true;
	}
	return errResp;
}