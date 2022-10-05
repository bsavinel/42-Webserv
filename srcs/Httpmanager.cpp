#include "HttpManager.hpp"


HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_init = false;
	_isEnd = false;
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

HttpManager::receive()
{
	
}

bool	HttpManager::applyMethod(int &stock, t_epoll_event &event)
{
	if (!_init)
	{
        _init = true;
		initialize();
	}
	else
	{
		if (_method == GET)
		{}
		else if (_method == POST)
		{}
		else if (_method == DELETE)
		{}
	}
	return _isEnd;
}
