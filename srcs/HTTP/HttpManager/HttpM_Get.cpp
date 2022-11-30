/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_Get.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:48 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 13:51:47 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"
#include "autoIndex.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <utils.hpp>

bool	HttpManager::autoIndexRequired()
{
	std::string name;

	name = buildLocalPath(_request);
	if (name[name.size() - 1] == '/' && _request.getLocation()->getAutoPath() && _request.getLocation()->getIndexPath().empty())
		return true;
	return false;
}

std::string HttpManager::retrieveCorrespondingLocalPath()
{
	std::string name_file;

	name_file = buildLocalPath(_request);
	if (_request.getUrl().first == _request.getLocation()->getLocate())
		name_file += _request.getLocation()->getIndexPath();
	return name_file;
}

void	HttpManager::openFileGET(std::string &file_name)
{
	_file_fd = open(file_name.c_str(), O_RDONLY);
	if (_file_fd< 0)
	{
		if (errno == EACCES)
			_errorCode = 403;
		else if (errno == EFAULT || errno == EMFILE || errno == ENFILE || errno == ENOMEM)
			_errorCode = 500;
		else if (errno == EFBIG || errno == EISDIR || errno == ELOOP || errno == ENAMETOOLONG || errno == ENODEV || errno == ETXTBSY)
			_errorCode = 400;
		else if (errno == ENOENT)
			_errorCode = 404;
	}
}

void	HttpManager::initGET()
{
	struct stat status;

	if (1) // ? condition autoindex
	{	
		_name_file = retrieveCorrespondingLocalPath();
		openFileGET(_name_file);
		if (_errorCode == 0)
		{
			stat(_name_file.c_str(), &status);
			if (!S_ISREG(status.st_mode))
			{
				_errorCode = 400;
				close(_file_fd);
				_file_fd= -1;
			}
		}
		if (_errorCode == 0)
			_respond = HeaderRespond(status.st_size, 200, determinateType(_name_file));
	}
	_headerBuild = true;
}

void HttpManager::buildGetRespond()
{
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	nb_char = read(_file_fd, buffer, LEN_TO_READ);
	if (nb_char > 0)
		_respond.insert(_respond.size(), &buffer[0], nb_char);
	if (nb_char == -1)
	{
		_errorCode = 501;
	}
	else if (nb_char < LEN_TO_READ)
		_isEnd = true;
}

void HttpManager::methodGET(const Server &server)
{
	std::string header;

	canWrite();
	if (_headerBuild == false)
	{
		_boolAutoIndex = autoIndexRequired();
		if (!_boolAutoIndex)
			initGET();
		_headerBuild = true;
	}
	else
	{
		if (_boolAutoIndex)
		{
			_respond = autoIndex(_request, *this);
			if (_errorCode == 0)
			{
				header = HeaderRespond(_respond.size(), 200, "text/html");
				_respond = header + _respond;
			}
			else
			{
				_respond.clear();
				_respond = ErrorRespond(server);
			}
			_isEnd = true;
		}
		else
			buildGetRespond();
	}
	if (_isEnd == true && _file_fd!= -1)
		close(_file_fd);
}
