#include "HttpManager.hpp"
#include <sys/socket.h>
#include <iostream>
#include <sstream>

void	autoIndex(HttpRequest &request, HttpManager &manager);

HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_RedirectionStart = false;
	_Readok = true;
	_modeChange = false;
	_init = false;
	_isEnd = false;
	_Writeok = false;
	_headerBuild = false;
	_errorCode = 0;
	_file = -1;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{
		_RedirectionStart = rhs._RedirectionStart;
		_errorCode = rhs._errorCode;
		_server = rhs._server;
		_socketClient = rhs._socketClient;
		_Writeok = rhs._Writeok;
		_Readok = rhs._Readok;
		_modeChange = rhs._modeChange;
		_init = rhs._init;
		_isEnd = rhs._isEnd;
		_file = rhs._file;
		_name_file = rhs._name_file;
		_headerBuild = rhs._headerBuild;
		_respond = rhs._respond;
		_request = rhs._request;
	}
	return *this;
}

HttpManager::~HttpManager()
{

}

void	HttpManager::sender()
{
	if (_respond.size() > 0)
	{
		send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		_respond.clear();
	}
}

int HttpManager::receive()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
	if ((ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT)) == -1)
		return (-1);
	_request.concatenate(buffer);
	//std::cout << buffer << std::endl;
	return (0);
}

bool	HttpManager::applyMethod(const Server &server)
{
	if (!_isEnd)
	{
		if (_errorCode == 0 && _request.getLocation()->getReturnCode() != 0)
		{
			canWrite();
			if (!redirectionManage())
				_errorCode = _request.getLocation()->getReturnCode();
			return _isEnd;
		}
		else if (_errorCode != 0)
		{
			_respond.clear();
			_respond = ErrorRespond(server);
		}
		else if (_request.getMethod().first == "GET")
			getMethod(server);
		else if (_request.getMethod().first == "POST")
			postMethod();
		else if (_request.getMethod().first == "DELETE")
			deleteMethod();
		else
			_isEnd = true;
	}
	return _isEnd;
}

void	HttpManager::initialize(const Server &server)
{
	if (!_init)
	{
		_init = true;
		_request.parser();
		_request.setLocation(_request.findLocation(server));
	}
}


void	HttpManager::canRead()
{
	if (_Readok == false)
	{
		_Readok = true;
		_modeChange = true;
	}
}

void	HttpManager::canWrite()
{
	if (_Writeok == false)
	{
		_Writeok = true;
		_modeChange = true;
	}
}

std::string HttpManager::determinateType(const std::string &name_file)
{
	if (name_file.rfind(".html") == name_file.size() - 5 && name_file.size() >= 5)
		return "text/html";
	else if (name_file.rfind(".css") == name_file.size() - 4 && name_file.size() >= 4)
		return  "text/css";
	else if (name_file.rfind(".ico") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/x-icon";
	else if (name_file.rfind(".png") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/png";
	else if (name_file.rfind(".jpeg") == name_file.size() - 5 && name_file.size() >= 5)
		return "image/jpeg";
	if (_errorCode == 0)
		_errorCode = 415;
	return "";	
}
