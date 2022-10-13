#include "HttpManager.hpp"
#include <sys/socket.h>
#include <iostream>

HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_Readok = true;
	_modeChange = false;
	_init = false;
	_isEnd = false;
	_endGet = false;
	_Writeok = false;
	_headerBuild = false;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{
        _socketClient = rhs._socketClient;
        _init = rhs._init;
        _Writeok = rhs._Writeok;
        _request = rhs._request;
		_headerBuild = rhs._headerBuild;
		_file = rhs._file;
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

bool	HttpManager::applyMethod()
{
	if (!_isEnd)
	{
		if (_request.getMethod().first == "GET")
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

void	HttpManager::parseHeader( void )
{
	_request.parser();
	/*std::cout << "##################################" << std::endl;
	std::cout << _request << std::endl;
	std::cout << "##################################" << std::endl;*/
}

void	HttpManager::initialize(const Server &server)
{
	if (!_init)
	{
		_init = true;
		parseHeader();
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