#include "HttpManager.hpp"
#include <sys/socket.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>

void	autoIndex(HttpRequest &request, HttpManager &manager);

HttpManager::HttpManager(t_socket socketClient)
{
	_goodRequest = false;
	_socketClient = socketClient;
	_RedirectionStart = false;
	_Readok = true;
	_modeChange = false;
	_init = false;
	_isEnd = false;
	_Writeok = false;
	_headerBuild = false;
	_errorCode = 0;
	_file = -1;
	_tmp_upload_fd = -1;
	_tmpEnd = false;
	_firstPassage = false;
	_proccess_fini = false;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{
		_goodRequest = rhs._goodRequest;
		_RedirectionStart = rhs._RedirectionStart;
		_socketClient = rhs._socketClient;
		_Writeok = rhs._Writeok;
		_proccess_fini = rhs._proccess_fini;
		_firstPassage = rhs._firstPassage;
		_errorCode = rhs._errorCode;
		_Readok = rhs._Readok;
		_modeChange = rhs._modeChange;
		_init = rhs._init;
		_isEnd = rhs._isEnd;
		_file = rhs._file;
		_name_file = rhs._name_file;
		_headerBuild = rhs._headerBuild;
		_respond = rhs._respond;
        _request = rhs._request;
		_tmp_upload_fd = rhs._tmp_upload_fd;
	}
	return *this;
}

HttpManager::~HttpManager()
{

}

void	HttpManager::sender()
{
	int ret;

	if (_respond.size() > 0)
	{
		ret = send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		_respond.clear();
		if (ret == -1)
			_isEnd = true;
	}
}

int HttpManager::receive()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
	ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT);
	if (ret == -1)
	{
		_isEnd = true;
		return -1;
	}
	_request.concatenate(buffer);
	//std::cout << buffer << std::endl;
	return (0);
}

/*std::string	HttpManager::ErrorRespond()
{
	std::string errResp;
	Error err;

	if (_errorCode == 204)
		errResp = "HTTP/1.1 204 No Content";
	else
	{
		errResp = buildErrorPage(_errorCode);
		errResp.insert(0, HeaderRespond(errResp.size(), _errorCode, "text/html"));
	}
	_isEnd = true;
	return errResp;
}*/

void	HttpManager::launch_cgi(HttpRequest &_request, const Server &server)
{
	struct stat _status;

	if (_tmpEnd == true)
		_isEnd = true;
	else if (!_tmpEnd)
	{
		canWrite();
		std::string header; 
		_cgi.initialise_env(_request, server);
		_cgi.set_path_cgi(_request.getLocation()->getCgiPathToScript());
		_cgi.set_argv();
		stat(_cgi.getScriptPath().c_str(), &_status);
		if(errno == ENOENT)
		{
			_errorCode = 404;
			_tmpEnd = true;
		}
		else
		{
			if (_firstPassage == false)
			{
				_firstPassage = true;
				_cgi.execute();
			}
			else
			{
				_respond.clear();
				if (_proccess_fini == false)
				{
					int retfo;
					retfo = _cgi.feedOutput();
					if (retfo == 1)
					{
						std::cout << _cgi.getOutput().size() << std::endl;
						_respond = HeaderRespond(_cgi.getOutput().size(), 200, "text/html");
						_proccess_fini = true;
						//std::cout <<"CGI OUT PUT = " << std::endl << _cgi.getOutput() << std::endl;
					}
					else if (retfo == -1)
						_errorCode = 408;
				}
				else
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
					{
						_isEnd = true; // peut mem directement is end
					}
				}
			}
		}
		//std::cout << "RESPOND CGI =" << std::endl << _respond << std::endl;
		
	}
}

bool	HttpManager::applyMethod(const Server &server)
{
	(void)server;
	if (!_isEnd)
	{
		if (_goodRequest == false)
		{
			_goodRequest = true;
            canWrite();
		}
		else if (_errorCode == 0 && _request.getLocation()->getReturnCode() != 0)
		{
			canWrite();
			if (!redirectionManage())
				_errorCode = _request.getLocation()->getReturnCode();
			return _isEnd;
		}
		else if (_errorCode != 0)
		{
			_respond.clear();
			std::cout << "ICI" << _errorCode << std::endl;
			_respond = ErrorRespond(server);
		}
		else if(!check_if_method_authorized())
			_errorCode = 405;
		else if(_request.getLocation()->getCgiFileExtension() == get_file_extension(_request.getUrl().first))
			launch_cgi(_request, server);
		else if (_request.getMethod().first == "GET")
			getMethod(server);
		else if (_request.getMethod().first == "POST")
		{
			std::cout << _request.getRequest() << std::endl;
			postMethod();
		}
		else if (_request.getMethod().first == "DELETE")
			deleteMethod();
		else
			_isEnd = true;
	}
	return _isEnd;
}

void	HttpManager::initialize(const Server &server)
{
	if (!_init)
	{
		_init = true;
		_request.parser();
		_request.setLocation(_request.findLocation(server));
		_goodRequest = checkRequest(server);
	}
}

bool	HttpManager::checkRequest(const Server &server)
{
	(void)server;
	if (_request.getHttpVersion().first != "HTTP/1.1\r" && _request.getHttpVersion().first != "HTTP/1.1")
		_errorCode = 505;
	else if (_request.getMethod().first != "GET" &&
			_request.getMethod().first != "POST" && 
			_request.getMethod().first != "DELETE")
		_errorCode = 501;
	/*else if (_request.getLocation()->getAllowedMethods().find(_request.getMethod()) == _request.getLocation()->getAllowedMethods().end())
		_errorCode = 405;*/
	/*else if (_request.getContentLenght().second == true && _request.getContentLenght().first > server.getClientMaxBodySize())
		_errorCode = 413*/
	else
		return true; 
	return false;
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

std::string HttpManager::determinateType(const std::string &name_file)
{
	if (name_file.rfind(".html") == name_file.size() - 5 && name_file.size() >= 5)
		return "text/html";
	else if (name_file.rfind(".css") == name_file.size() - 4 && name_file.size() >= 4)
		return  "text/css";
	else if (name_file.rfind(".ico") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/x-icon";
	else if (name_file.rfind(".png") == name_file.size() - 4 && name_file.size() >= 4)
		return "image/png";
	else if (name_file.rfind(".jpeg") == name_file.size() - 5 && name_file.size() >= 5)
		return "image/jpeg";
	if (_errorCode == 0)
		_errorCode = 415;
	return "";	
}

bool	HttpManager::check_if_method_authorized()
{
	std::vector<std::string>::const_iterator itMethod = _request.getLocation()->getAllowedMethods().begin();
	std::vector<std::string>::const_iterator iteMethod = _request.getLocation()->getAllowedMethods().end();

	while (itMethod != iteMethod)
	{
		if(_request.getMethod().first == (*itMethod))
			return (1);
		itMethod++;
	}
	return (0);
}