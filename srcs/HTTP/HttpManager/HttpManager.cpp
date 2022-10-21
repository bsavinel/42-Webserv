#include "HttpManager.hpp"
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include "Error.hpp"

void	autoIndex(HttpRequest &request);

HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
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
	return (0);
}

std::string	HttpManager::ErrorRespond()
{
	std::string errResp;
	Error err;

	if (_request.getMethod().first == "GET")
	{
		errResp = buildErrorPage(_errorCode);
		errResp.insert(0, HeaderRespond(errResp.size(), _errorCode, "text/html"));
	}
	if (_request.getMethod().first == "DELETE")
	{
		std::stringstream ss;
		ss << _errorCode;
		errResp = "HTTP/1.1 " + ss.str() + " " + err.getError(_errorCode) + "\n\n";
	}
	_isEnd = true;
	return errResp;
}

bool	HttpManager::applyMethod()
{
	if (!_isEnd)
	{
		if (_errorCode != 0)
			_respond = ErrorRespond();
		else if (_request.getMethod().first == "GET")
			getMethod();
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
		std::cout << _request.getRequest();
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

std::string HttpManager::determinateType()
{
	if (_name_file.rfind(".html") == _name_file.size() - 5 && _name_file.size() >= 5)
		return "text/html";
	else if (_name_file.rfind(".css") == _name_file.size() - 4 && _name_file.size() >= 4)
		return  "text/css";
	else if (_name_file.rfind(".ico") == _name_file.size() - 4 && _name_file.size() >= 4)
		return "image/x-icon";
	else if (_name_file.rfind(".png") == _name_file.size() - 4 && _name_file.size() >= 4)
		return "image/png";
	else if (_name_file.rfind(".jpeg") == _name_file.size() - 5 && _name_file.size() >= 5)
		return "image/jpeg";
	_errorCode = 415;
	return "";	
}

std::string HttpManager::buildLocalPath()
{
	std::string	localPath;
	const std::string &locationPath = _request.getLocation()->getLocate();
	const std::string &UrlPath = _request.getUrl().first;
	const std::string &RootPath = _request.getLocation()->getRootPath();

	localPath.insert(0, UrlPath, locationPath.size(), UrlPath.size() - locationPath.size());
	if (localPath[0] == '/')
		localPath.erase(0, 1);
	localPath.insert(0, RootPath);
	return localPath;
}