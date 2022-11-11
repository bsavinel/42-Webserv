#include "HttpManager.hpp"
#include "Error.hpp"
#include <sys/stat.h>

std::string	HttpManager::buildSimpleErrorResponse()
{
	std::string errResp;

	if (_errorCode == 0)
		return errResp;
	else if (_errorCode == 204)
		errResp = "HTTP/1.1 204 No Content\n\n";
	else
	{
		errResp = buildErrorPage(_errorCode);
		errResp.insert(0, HeaderRespond(errResp.size(), _errorCode, "text/html"));
	}
	return errResp;
}

bool	HttpManager::init_error_file(const std::string &error_page, std::string &errResp)
{
	struct stat status;

	_file = open(error_page.c_str(), O_RDONLY);
	stat(error_page.c_str(), &status);
	if (_file < 0 || !S_ISREG(status.st_mode))
	{
		if (_file >= 0)
			close(_file);
		if (_errorCode == 0)
			return false;
		_isEnd = true;
		errResp = buildSimpleErrorResponse();
		return false;
	}
	std::string type_file = determinateType(error_page);
	if (type_file.empty())
	{
		errResp = buildSimpleErrorResponse();
		_isEnd = true;
		return false;
	}
	errResp = HeaderRespond(status.st_size, _errorCode, type_file);
	return true;
}

std::string	HttpManager::ErrorRespond(const Server &server)
{
	std::string errResp;
	Error err;
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	canWrite();
	if (server.getErrorMap().find(_errorCode) != server.getErrorMap().end())
	{
		std::cout << "ICI" << _errorCode << std::endl;

		if (_file == -1)
            if (!init_error_file((*server.getErrorMap().find(_errorCode)).second, errResp))
				return errResp;
		std::cout << "ap" << _errorCode << std::endl;

		nb_char = read(_file, buffer, LEN_TO_READ);
		if (nb_char > 0)
			errResp.insert(errResp.size(), &buffer[0], nb_char);
		if (nb_char < LEN_TO_READ)
		{
			close(_file);
			_isEnd = true;
		}
	}
	else
	{
		std::cout << "la" << _errorCode << std::endl;

		errResp = buildSimpleErrorResponse();
		_isEnd = true;
	}
	return errResp;
}