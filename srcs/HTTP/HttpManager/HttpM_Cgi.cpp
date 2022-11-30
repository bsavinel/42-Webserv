/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_Cgi.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:39 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 13:51:58 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"
#include <sys/stat.h>
#include <iostream>

void	HttpManager::launchCgi()
{
	if (_firstPassage == false)
	{
		_firstPassage = true;
		try
		{
			if(!_cgi.execute())
			{
				std::cout << "Ca foire ici " << std::endl;
				_errorCode = 401;
			}
		}
		catch(const std::exception& e)
		{
			_errorCode = 500;
			std::cerr << e.what() << '\n';
		}
	}
	else
	{
		_respond.clear();
		if (_proccess_fini == false)
			checkIfProcessIsFinished();
		else
			fillResponseWithCgiOutput();
	}
}


void	HttpManager::manageCgi(HttpRequest &_request, const Server &server)
{
	struct stat _status;
	std::string path = retrieveCorrespondingLocalPath();

	if (_tmpEnd == true)
	{
		_cgi.free_argenv();
		_isEnd = true;
	}
	else if (!_tmpEnd)
	{
		canWrite();
		std::string header;
		_cgi.initialise_env(_request, server, path);
		_cgi.set_path_cgi(_request.getLocation()->getCgiPathToScript());
		_cgi.set_argv();
		stat(_cgi.getScriptPath().c_str(), &_status);
		if (errno == ENOENT)
		{
			_errorCode = 404;
			_tmpEnd = true;
		}
		else
			launchCgi();
	}
}

void		HttpManager::checkIfProcessIsFinished()
{
		int retfo;
		retfo = _cgi.feedOutput();
		if (retfo == 1)
		{
			_respond = HeaderRespond(_cgi.getOutput().size(), 200, "text/html", _cgi.getCookies());
			_proccess_fini = true;
		}
		else if (retfo == -1)
			_errorCode = 408;
}


void	HttpManager::fillResponseWithCgiOutput()
{
	if (_respond.size() > LEN_TO_READ)
	{
		_respond.insert(_respond.size(), _cgi.getOutput(), 0, LEN_TO_READ);
		_cgi.cutOutput(LEN_TO_READ);
	}
	else
	{
		_respond.insert(_respond.size(), _cgi.getOutput(), 0, _cgi.getOutput().size());
		_cgi.cutOutput(_cgi.getOutput().size());
	}
	if (_cgi.getOutput().size() == 0)
		_isEnd = true; 
}