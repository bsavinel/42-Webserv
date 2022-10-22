#include "HttpManager.hpp"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
void	autoIndex(HttpRequest &request);

HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_Readok = true;
	_modeChange = false;
	_init = false;
	_isEnd = false;
	_Writeok = false;
	_headerBuild = false;
	_file = -1;
	_tmp_upload_fd = -1;
	_requestFullyReceive = false;
	_tmpEnd = false;
	_lenRead = 0;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{
		_server = rhs._server;
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
		_requestFullyReceive = rhs._requestFullyReceive;
		_tmpEnd = rhs._tmpEnd;
		_lenRead = rhs._lenRead;
	}
	return *this;
}

HttpManager::~HttpManager()
{

}

void	HttpManager::sender()
{
	int ret = 0;

	if (_respond.size() > 0)
	{
//		std::cout << "size:" << _respond.size() << std::endl << "RESPONSE:" << std::endl  << _respond.c_str() << "/RESPONSE" << std::endl;
		ret = send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		if (ret == -1)
			strerror(errno);
		else
			std::cout << "ret send:" << ret << std::endl;
		_respond.clear();
	}

}

int HttpManager::receive()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
//	usleep(1000);
	if ((ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT)) == -1)
		return (-1);
	std::cout << "ret = " << ret << std::endl;
	_lenRead += ret;
	std::cout << _lenRead << " : " << _request.getContentLength().first;
	if (_request.getContentLength().second == true &&  _lenRead >= _request.getContentLength().first)
	{
		std::cout <<  "Request FULLY READ : " << ret << std::endl;

		std::string buff(buffer);
		std::cout <<  "BUFFER=\n" << buffer << "\n=BUFFER" << std::endl;
		_requestFullyReceive = true;
	}
	_request.concatenate(buffer);
//	std::cout <<  "REQUEST=\n" << _request.getRequest().c_str() << "\n=REQUEST" << std::endl;
	return (0);
}

bool	HttpManager::applyMethod()
{
	if (!_isEnd)
	{
		if (_request.getMethod().first == "GET")
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