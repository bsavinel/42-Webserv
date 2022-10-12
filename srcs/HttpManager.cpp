#include "HttpManager.hpp"
#include <sys/socket.h>

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

HttpManager::HttpManager(t_socket socketClient)
{
	_socketClient = socketClient;
	_init = false;
	_isEnd = false;
	_Readok = true;
	_Writeok = true;
}

HttpManager::HttpManager(const HttpManager& rhs)
{
	*this = rhs;
}

HttpManager		&HttpManager::operator=(const HttpManager& rhs)
{
	if (this != &rhs)
	{
        _socketClient = rhs._socketClient;
        _init = rhs._init;
        _isEnd = rhs._isEnd;
        _Readok = rhs._Readok;
        _Writeok = rhs._Writeok;
        _request = rhs._request;
	}
	return *this;
}

HttpManager::~HttpManager()
{

}

void	HttpManager::setReadOk(bool read)
{
	_Readok = read;
}

void	HttpManager::setWriteOk(bool write)
{
	_Writeok = write;
}

void	HttpManager::sender()
{
	if (_respond.size() > 0)
	{
		send(_socketClient, _respond.c_str(), _respond.size() - 1, MSG_NOSIGNAL);
	}
	_respond.empty();
}

int HttpManager::receive()
{
	int ret;
	char buffer[LEN_TO_READ + 1];

	//std::cout << "passage dans recev" << std::endl;
	for (int i = 0; i < LEN_TO_READ + 1; i++)
		buffer[i] = 0;
	if ((ret = recv(_socketClient, buffer, LEN_TO_READ, MSG_DONTWAIT)) == -1)
		return (-1);
	std::cout << buffer << std::endl;
	_request.concatenate(buffer);
	return (0);
}

bool	HttpManager::applyMethod(int &stock, t_epoll_event &event)
{
	(void)(event);
	(void)(stock);
	if (!_init)
	{
		_init = true;
		std::cout << "debut parseHeader" << std::endl;
		parseHeader();
		std::cout << "fin parseHeader" << std::endl;
	}
	//else
	//{
		if (_request.getMethod().first == "GET")
			getMethod();
		else if (_request.getMethod().first == "POST")
			postMethod();
		else if (_request.getMethod().first == "DELETE")
			deleteMethod();
		else
			_isEnd = true;
	//}
	return _isEnd;
}

void	HttpManager::parseHeader( void )
{
	_request.parser();
}


void buildHeader(const HttpRequest &request, std::string &header ,off_t size)
{
	header += "HTTP/1.1 200 OK\n"; // TODO voir les image apres
	if (request.getUrl().first.find("html") != std::string::npos)
        header += "Content-Type: text/html\n";
	else if (request.getUrl().first.find("css") != std::string::npos)
        header += "Content-Type: text/css\n";
	else if (request.getUrl().first.find("ico") != std::string::npos)
        header += "Content-Type: image/x-icon\n";
	// TODO changer cette merde
	std::stringstream ss;
	ss << size;

	header += "Content-Length: " + ss.str() + "\n";
    header += "\n";
}

void	HttpManager::getMethod()
{
	char buffer[1024];
	struct stat status;
	int nb_char = 1;
	int _file;

	if (_request.getUrl().first.compare("/") == 0)
	{
		_file = open("./data/www/index.html", O_RDONLY);
	}
	else
	{
		std::string root("./data/www/" + _request.getUrl().first);
		_file = open(root.c_str(), O_RDONLY);
	}
	if (_file < 0)
        return ;
	if (fstat(_file, &status) == -1)
	{
        close(_file); 
        return ;
	}
	std::string header;
	buildHeader(_request, header ,status.st_size);
	std::cout << header;
	send(_socketClient, header.c_str(), header.size(), MSG_NOSIGNAL);
	while (nb_char)
	{
		nb_char = read(_file, buffer, 1024);
		if (nb_char > 0)
		{
			std::cout << buffer << std::endl;
			send(_socketClient, buffer, nb_char, MSG_NOSIGNAL);
		}
	}
	close(_file);
	_isEnd = true;
}
void	HttpManager::postMethod()
{

}
void	HttpManager::deleteMethod()
{

}