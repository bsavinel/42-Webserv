#include "HttpManager.hpp"
#include "Error.hpp"
#include <sys/stat.h>

std::string	HttpManager::buildSimpleErrorResponse()
{
	std::string errResp;

	if (_errorCode == 204)
		errResp = "HTTP/1.1 204 No Content\n\n";
	else
	{
		errResp = buildErrorPage(_errorCode);
		errResp.insert(0, HeaderRespond(errResp.size(), _errorCode, "text/html"));
	}
	return errResp;
}

bool	HttpManager::init_error_file(const Server &server, std::string &errResp)
{
	struct stat status;

	_file = open((*server.getErrorMap().find(_errorCode)).second.c_str(), O_RDONLY);
	stat((*server.getErrorMap().find(_errorCode)).second.c_str(), &status);
	if (_file < 0 || !S_ISREG(status.st_mode))
	{
		std::cerr << "open file for error: " << _errorCode << " failed" << std::endl;
		errResp = buildSimpleErrorResponse();
		_isEnd = true;
		return false;
	}
	std::string type_file = determinateType((*server.getErrorMap().find(_errorCode)).second);
	if (type_file.empty())
	{
		std::cerr << "unknown file exension for error : " << _errorCode << std::endl;
		errResp = buildSimpleErrorResponse();
		_isEnd = true;
		return false;
	}
	errResp = HeaderRespond(status.st_size, 200, type_file);
	return true;
}

std::string	HttpManager::ErrorRespond(const Server &server)
{
	std::string errResp;
	Error err;
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	if (server.getErrorMap().find(_errorCode) != server.getErrorMap().end())
	{
		if (_file == -1)
            if (!init_error_file(server, errResp))
				return errResp;
		nb_char = read(_file, buffer, LEN_TO_READ);
		if (nb_char > 0)
			errResp.insert(errResp.size(), &buffer[0], nb_char);
		if (nb_char < LEN_TO_READ)
			_isEnd = true;
	}
	else
	{
		errResp = buildSimpleErrorResponse();
		_isEnd = true;
	}
	return errResp;
}