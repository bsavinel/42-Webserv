/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:00 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 13:39:52 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"
# include "HttpM_Post.hpp"
#include <sys/socket.h>

#include <iostream>
#include <sstream>
#include <unistd.h>

void autoIndex(HttpRequest &request, HttpManager &manager);

HttpManager::HttpManager(t_socket socketClient)
{
	_goodRequest = false;
	_socketClient = socketClient;
	_RedirectionStart = false;
	_Readok = true;
	_modeChange = false;
	_init = false;
	_isEnd = false;
	_Writeok = false;
	_headerBuild = false;
	_errorCode = 0;
	_file_fd= -1;
	_tmp_upload_fd = -1;
	_tmpEnd = false;
	_firstPassage = false;
	_proccess_fini = false;
	_requestFullyReceive = false;
	_lenRead = 0;
	_new_process = false;
	_lenOfRequestAlreadyRead = 0;
	_process = createProcess();
}

HttpManager::HttpManager(const HttpManager &rhs)
{
	*this = rhs;
}

HttpManager &HttpManager::operator=(const HttpManager &rhs)
{
	if (this != &rhs)
	{
		_goodRequest = rhs._goodRequest;
		_RedirectionStart = rhs._RedirectionStart;
		_socketClient = rhs._socketClient;
		_Writeok = rhs._Writeok;
		_proccess_fini = rhs._proccess_fini;
		_firstPassage = rhs._firstPassage;
		_errorCode = rhs._errorCode;
		_Readok = rhs._Readok;
		_modeChange = rhs._modeChange;
		_init = rhs._init;
		_isEnd = rhs._isEnd;
		_file_fd= rhs._file_fd;
		_name_file = rhs._name_file;
		_headerBuild = rhs._headerBuild;
		_respond = rhs._respond;
		_request = rhs._request;
		_tmp_upload_fd = rhs._tmp_upload_fd;
		_requestFullyReceive = rhs._requestFullyReceive;
		_tmpEnd = rhs._tmpEnd;
		_lenRead = rhs._lenRead;
		_lenOfRequestAlreadyRead = rhs._lenOfRequestAlreadyRead;
		_new_process = rhs._new_process;
		_process = rhs._process;
	}
	return *this;
}

HttpManager::~HttpManager()
{
}


void HttpManager::sender()
{
	int ret;

	if (_respond.size() > 0)
	{
		ret = send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		if (ret == -1)
		{
			_isEnd = true;
		}
		// std::cout << _respond << std::endl;
		_respond.clear();
		if (ret == -1)
			_isEnd = true;
	}

}
int HttpManager::receiver()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
	if ((ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT)) == -1)
	{
		_isEnd = true;
		return (-1);
	}
	_lenRead += ret;
	if (_request.getContentLength().second == true &&  _lenRead >= _request.getContentLength().first)
	{
		_requestFullyReceive = true;
	}
	_request.concatenateInsert(buffer, ret);
	return (0);
}

void HttpManager::initialize(const Server &server)
{
	if (_request.getRequest().find("\r\n\r\n") == std::string::npos)
		return ;
	if (!_init)
	{
		_init = true;
		_request.parser();
		_lenRead = 0;
		_lenRead = _request.getRequest().size();
		_request.setLocation(_request.findLocation(server));
		_goodRequest = checkRequest(server);
	}
}

bool HttpManager::checkIfMethodIsAthorized()
{
	std::vector<std::string>::const_iterator itMethod = _request.getLocation()->getAllowedMethods().begin();
	std::vector<std::string>::const_iterator iteMethod = _request.getLocation()->getAllowedMethods().end();

	while (itMethod != iteMethod)
	{
		if (_request.getMethod().first == (*itMethod))
			return (1);
		itMethod++;
	}
	return (0);
}

void HttpManager::canRead()
{
	if (_Readok == false)
	{
		_Readok = true;
		_modeChange = true;
	}
}

void HttpManager::canWrite()
{
	if (_Writeok == false)
	{
		_Writeok = true;
		_modeChange = true;
	}
}

bool HttpManager::checkRequest(const Server &server)
{
	(void)server;
	if (_request.getHttpVersion().first != "HTTP/1.1\r" && _request.getHttpVersion().first != "HTTP/1.1")
		_errorCode = 505;
	else if (_request.getMethod().first != "GET" &&
			 _request.getMethod().first != "POST" &&
			 _request.getMethod().first != "DELETE")
		_errorCode = 501;
	else if (!checkIfMethodIsAthorized())
		_errorCode = 405;
	else if (server.getClientMaxBodySize() != 0 && 
				(_request.getContentLength().second == true && _request.getContentLength().first > server.getClientMaxBodySize()))
		_errorCode = 413;
	else
		return true;
	return false;
}

std::string HttpManager::determinateType(const std::string &name_file)
{
	if (name_file.rfind(".html") == name_file.size() - 5 && name_file.size() >= 5)
		return "text/html";
	else if (name_file.rfind(".css") == name_file.size() - 4 && name_file.size() >= 4)
		return "text/css";
	else if (name_file.rfind(".ico") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/x-icon";
	else if (name_file.rfind(".png") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/png";
	else if (name_file.rfind(".jpeg") == name_file.size() - 5 && name_file.size() >= 5)
		return "image/jpeg";
	else if (name_file.rfind(".jpg") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/jpg";
	if (_errorCode == 0)
		_errorCode = 415;
	return "";
}

bool HttpManager::applyMethod(const Server &server)
{
	(void)server;
	if (!_isEnd && _init)
	{
		if (_goodRequest == false)
		{
			_goodRequest = true;
			canWrite();
		}
		else if (_errorCode == 0 && _request.getLocation()->getReturnCode() != 0)
		{
			canWrite();
			if (!manageRedirection())
				_errorCode = _request.getLocation()->getReturnCode();
			return _isEnd;
		}
		else if (_errorCode != 0)
		{
			_respond.clear();
			_respond = ErrorRespond(server);
		}
		else if (!checkIfMethodIsAthorized())
			_errorCode = 405;
		else if (!_request.getLocation()->getCgiFileExtension().empty() && _request.getLocation()->getCgiFileExtension() == get_file_extension(retrieveCorrespondingLocalPath()))
			manageCgi(_request, server);
		else if (_request.getMethod().first == "GET")
			methodGET(server);
		else if (_request.getMethod().first == "POST")
			methodPOST();
		else if (_request.getMethod().first == "DELETE")
			methodDELETE();
		else
			_isEnd = true;
	}
	return _isEnd;
}
