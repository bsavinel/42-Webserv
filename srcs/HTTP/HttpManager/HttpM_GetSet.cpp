#include "HttpManager.hpp"
#include <iostream>

const bool	&HttpManager::getInit() const
{
	return _init;
}

const bool	&HttpManager::getWriteOk() const
{
	return _Writeok;
}

const bool	&HttpManager::getReadOk() const
{
	return _Readok;
}

const bool	&HttpManager::getIsEnd() const
{
	return _isEnd;
}

const HttpRequest &HttpManager::getRequest() const
{
	return _request;
}

const std::string &HttpManager::getResponse() const
{
	return _respond;
}

const bool	&HttpManager::getModeChange() const
{
	return _modeChange;
}

void	HttpManager::setModeChange(bool modeChange)
{
	_modeChange = modeChange;
}
