#include "HttpManager.hpp"
#include "autoIndex.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

bool	HttpManager::autoIndexRequired()
{
	std::string name;

	name = buildLocalPath(_request);
	if (name[name.size() - 1] == '/' && _request.getLocation()->getAutoPath() && _request.getLocation()->getIndexPath().empty())
		return true;
	return false;
}

std::string HttpManager::LocalPathFile_get()
{
	std::string name_file;

	name_file = buildLocalPath(_request);
	if (_request.getUrl().first == _request.getLocation()->getLocate())
		name_file += _request.getLocation()->getIndexPath();
	return name_file;
}

void	HttpManager::OpenFile_get(std::string &file_name)
{
	// TODO check les droits
	_file = open(file_name.c_str(), O_RDONLY);
	if (_file < 0)
	{
		if (errno == EACCES)
			_errorCode = 403;
		else if (errno == EFAULT || errno == EMFILE || errno == ENFILE || errno == ENOMEM)
			_errorCode = 500;
		else if (errno == EFBIG || errno == EISDIR || errno == ELOOP || errno == ENAMETOOLONG || errno == ENODEV || errno == ETXTBSY)
			_errorCode = 400;
		else if (errno == ENOENT)
			_errorCode = 404;
	}
}

void	HttpManager::initialize_get()
{
	struct stat status;

	if (1) // ? condition autoindex
	{
		_name_file = LocalPathFile_get();
		OpenFile_get(_name_file);
		if (_errorCode == 0)
		{
			stat(_name_file.c_str(), &status);
			if (!S_ISREG(status.st_mode))
			{
				_errorCode = 400;
				close(_file);
			}
		}
		if (_errorCode == 0)
			_respond = HeaderRespond(status.st_size, 200, determinateType());
	}
	_headerBuild = true;
}

void HttpManager::builRespondGet()
{
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	nb_char = read(_file, buffer, LEN_TO_READ);
	if (nb_char > 0)
		_respond.insert(_respond.size(), &buffer[0], nb_char);
	if (nb_char < LEN_TO_READ)
		_isEnd = true;
}

void HttpManager::getMethod()
{
	std::string header;

	canWrite();
	if (_headerBuild == false)
	{
		_boolAutoIndex = autoIndexRequired();
		if (!_boolAutoIndex)
			initialize_get();
		_headerBuild = true;
	}
	else
	{
		if (_boolAutoIndex)
		{
			_respond = autoIndex(_request, *this);
			if (_errorCode == 0)
			{
				header = HeaderRespond(_respond.size(), 200, "text/html");
				_respond = header + _respond;
				_isEnd = true;
			}
			else
			{
				_respond.clear();
				_respond = ErrorRespond();
				_isEnd = true;
			}
		}
		else
			builRespondGet();
	}
	if (_isEnd == true && _file != -1)
		close(_file);
}
