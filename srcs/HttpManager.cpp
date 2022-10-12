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
	_Writeok = false;
	_headerBuild = false;
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
        _Writeok = rhs._Writeok;
        _request = rhs._request;
		_headerBuild = rhs._headerBuild;
		_file = rhs._file;
	}
	return *this;
}

HttpManager::~HttpManager()
{

}

bool	HttpManager::getIsEnd()
{
    return _isEnd;
}


bool	HttpManager::getWriteOk()
{
	return _Writeok;
}

void	HttpManager::sender()
{
	std::cout << "send de merde" << std::endl;
	if (_respond.size() > 0)
	{
		send(_socketClient, _respond.c_str(), _respond.size(), MSG_NOSIGNAL);
		_respond.empty();
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
	_request.concatenate(buffer);
	return (0);
}

bool	HttpManager::applyMethod(int &infoServer)
{
	(void)(infoServer);

	if (!_init)
	{
		_init = true;
		parseHeader();
	}
	if (_request.getMethod().first == "GET")
		getMethod(infoServer);
	else if (_request.getMethod().first == "POST")
		postMethod();
	else if (_request.getMethod().first == "DELETE")
		deleteMethod();
	else
		_isEnd = true;
	return _isEnd;
}

void	HttpManager::parseHeader( void )
{
	_request.parser();
}


void HttpManager::buildHeaderGet(off_t size)
{
	_respond += "HTTP/1.1 200 OK\n"; // TODO voir les image apres
	if (_request.getUrl().first.find("html") != std::string::npos)
        _respond += "Content-Type: text/html\n";
	else if (_request.getUrl().first.find("css") != std::string::npos)
        _respond += "Content-Type: text/css\n";
	else if (_request.getUrl().first.find("ico") != std::string::npos)
        _respond += "Content-Type: image/x-icon\n";
	if (_request.getUrl().first.compare("/") == 0)
		_respond += "Content-Type: text/html\n";
	// TODO changer cette merde
	std::stringstream ss;
	ss << size;

	_respond += "Content-Length: " + ss.str() + "\n";
    _respond += "\n";
	std::cout << "size : " << ss.str() << std::endl;
}

void	HttpManager::initialize_get(int &infoServer)
{
	(void)infoServer;
	struct stat status;

	//* open good file and create header + regarder page ererue necesaire a envoye

	// TODO faire un read de 0 pour voir si on peut bien read dessus
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
	buildHeaderGet(status.st_size);
	_headerBuild = true;
}

void	HttpManager::builRespondGet()
{
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	nb_char = read(_file, buffer, LEN_TO_READ);
	if (nb_char > 0)
		_respond.insert(_respond.size(), &buffer[0], nb_char);
	if (nb_char < LEN_TO_READ)
		_isEnd = true;
}

void	HttpManager::getMethod(int &infoServer)
{
	(void)infoServer;

	std::cout << "ici" << std::endl;
	_Writeok = true;
	if (_headerBuild == false)
	{
		initialize_get(infoServer);
	}
	builRespondGet();
	if (_isEnd == true)
		close(_file);


}
void	HttpManager::postMethod()
{

}
void	HttpManager::deleteMethod()
{

}

const std::string &HttpManager::getResponse() const
{
	return _respond;
}