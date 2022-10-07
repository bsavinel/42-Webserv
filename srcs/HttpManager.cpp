#include "HttpManager.hpp"
#include <sys/socket.h>


HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_init = false;
	_isEnd = false;
	_Readok = true;
	_Writeok = true;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{}
	return *this;
}

HttpManager::~HttpManager()
{

}

void	HttpManager::setReadOk(bool read)
{
	_Readok = read;
}

void	HttpManager::setWriteOk(bool write)
{
	_Writeok = write;
}

void	HttpManager::sender()
{
	if (_respond.size() > 0)
	{
		send(_socketClient, _respond.c_str(), _respond.size() - 1, MSG_NOSIGNAL);
	}
	_respond.empty();
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

bool	HttpManager::applyMethod(int &stock, t_epoll_event &event)
{
	(void)(event);
	(void)(stock);
	if (!_init)
	{
		_init = true;
		parseHeader();
	}
	else
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
}

void	HttpManager::getMethod()
{

}
void	HttpManager::postMethod()
{

}
void	HttpManager::deleteMethod()
{

}