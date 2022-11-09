#include "HttpManager.hpp"
#include <sys/socket.h>
#include <iostream>
#include <sstream>

void	autoIndex(HttpRequest &request, HttpManager &manager);

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
		_goodRequest = rhs._goodRequest;
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
	int ret;

	if (_respond.size() > 0)
	{
		ret = send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		_respond.clear();
		if (ret == -1)
			_isEnd = true;
	}
}

int HttpManager::receive()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
	ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT);
	if (ret == -1)
	{
		_isEnd = true;
		return -1;
	}
	_request.concatenate(buffer);
	//std::cout << buffer << std::endl;
	return (0);
}

bool	HttpManager::applyMethod(const Server &server)
{
	if (!_isEnd)
	{
		if (_goodRequest == false)
		{
			_goodRequest = true;
            canWrite();
		}
		else if (_errorCode == 0 && _request.getLocation()->getReturnCode() != 0)
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
		_goodRequest = checkRequest(server);
	}
}

bool	HttpManager::checkRequest(const Server &server)
{
	(void)server;
	if (_request.getHttpVersion().first != "HTTP/1.1\r" && _request.getHttpVersion().first != "HTTP/1.1")
		_errorCode = 505;
	else if (_request.getMethod().first != "GET" &&
			_request.getMethod().first != "POST" && 
			_request.getMethod().first != "DELETE")
		_errorCode = 501;
	/*else if (_request.getLocation()->getAllowedMethods().find(_request.getMethod()) == _request.getLocation()->getAllowedMethods().end())
		_errorCode = 405;*/
	/*else if (_request.getContentLenght().second == true && _request.getContentLenght().first > server.getClientMaxBodySize())
		_errorCode = 413*/
	else
		return true; 
	return false;
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
