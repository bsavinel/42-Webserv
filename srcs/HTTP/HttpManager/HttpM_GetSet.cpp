/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_GetSet.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:51 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:43:52 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

const HttpRequest	&HttpManager::getClassRequest() const
{
	return _request;
}

void	HttpManager::setModeChange(bool modeChange)
{
	_modeChange = modeChange;
}

void	HttpManager::setErrorCode(int errorCode)
{
	_errorCode = errorCode;
}

void	HttpManager::setIsEnd(bool isEnd)
{
	_isEnd = isEnd;
}

void	HttpManager::setInit(bool init)
{
	_init = init;
}
