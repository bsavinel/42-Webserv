#include "HttpManager.hpp"
#include <sys/socket.h>
#include <iostream>
#include <sstream>
#include "Error.hpp"

void	autoIndex(HttpRequest &request, HttpManager &manager);

HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_Readok = true;
	_modeChange = false;
	_init = false;
	_isEnd = false;
	_Writeok = false;
	_headerBuild = false;
	_errorCode = 0;
	_file = -1;
	_tmp_upload_fd = -1;
	// _cgi = new Cgi;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{
		_errorCode = rhs._errorCode;
        _socketClient = rhs._socketClient;
        _Writeok = rhs._Writeok;
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
	if (_respond.size() > 0)
	{
		send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		_respond.clear();
	}
}

int HttpManager::receive()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
	if ((ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT)) == -1)
		return (-1);
	std::string buff(buffer);
	std::cout <<  "BUFFER=" << buff << std::endl <<  "=BUFFER" << std::endl;
	_request.concatenate(buffer);
	return (0);
}

std::string	HttpManager::ErrorRespond()
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
}

bool	HttpManager::applyMethod(const Server &server)
{
	(void)server;
	if (!_isEnd)
	{
		if (_errorCode != 0)
		{
			_respond.clear();
			_respond = ErrorRespond();
		}
		else if(_request.getLocation()->getCgiFileExtension() == get_file_extension(_request.getUrl().first))
		{
			std::cout << "______CGI EXECUTION HERE______" << std::endl;
			_cgi.initialise_env(_request, server);
			//_cgi.printEnv();
			_cgi.set_argv();
			_cgi.printArg();
			_cgi.execute();
			//retour du cgi --> buff
			//stocker retour du cgi dans le sender ou le send
			//le reste se charge d envoyer
			_isEnd = true;
		}
		else if (_request.getMethod().first == "GET")
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

void	HttpManager::initialize(const Server &server)
{
	if (!_init)
	{
		_init = true;
		_request.parser();
		_request.setLocation(_request.findLocation(server));
	}
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

std::string HttpManager::determinateType()
{
	if (_name_file.rfind(".html") == _name_file.size() - 5 && _name_file.size() >= 5)
		return "text/html";
	else if (_name_file.rfind(".css") == _name_file.size() - 4 && _name_file.size() >= 4)
		return  "text/css";
	else if (_name_file.rfind(".ico") == _name_file.size() - 4 && _name_file.size() >= 4)
		return "image/x-icon";
	else if (_name_file.rfind(".png") == _name_file.size() - 4 && _name_file.size() >= 4)
		return "image/png";
	else if (_name_file.rfind(".jpeg") == _name_file.size() - 5 && _name_file.size() >= 5)
		return "image/jpeg";
	_errorCode = 415;
	return "";	
}
