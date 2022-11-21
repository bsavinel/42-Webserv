#include "HttpManager.hpp"
#include <sys/stat.h>


void	HttpManager::launchCgi()
{
	if (_firstPassage == false)
	{
		_firstPassage = true;
		if(!_cgi.execute())
			_errorCode = 401;
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

	if (_tmpEnd == true)
	{
		_cgi.free_argenv();
		_isEnd = true;
	}
	else if (!_tmpEnd)
	{
		canWrite();
		std::string header;
		_cgi.initialise_env(_request, server);
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