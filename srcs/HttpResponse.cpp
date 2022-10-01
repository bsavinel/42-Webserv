#include "HttpResponse.hpp"
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sstream>

HttpResponse::HttpResponse()
{
	_initialized = false;
}

HttpResponse::HttpResponse(t_socket fd)
{
	_initialized = false;
	this->_fd = fd;
}

HttpResponse::HttpResponse(const HttpResponse& rhs)
{
	*this = rhs;
}


HttpResponse::~HttpResponse()
{
}

HttpResponse &HttpResponse::operator=(const HttpResponse& rhs)
{
	this->_fd = rhs._fd;
	this->_file = rhs._file;
	this->_initialized = rhs._initialized;
	return *this;
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

bool HttpResponse::buildRespond(const HttpRequest &request, int maxOperation)
{
	(void)maxOperation;
	char buffer[1024];
	struct stat status;
	int nb_char = 1;

	if (request.getMethod().first.compare("GET") == 0)
    {
		if (!_initialized)
		{
			if (request.getUrl().first.compare("/") == 0)
			{
				_file = open("./ressources/index.html", O_RDONLY);
			}
			else
			{
				std::string truc("./ressources/" + request.getUrl().first);
				_file = open(truc.c_str(), O_RDONLY);
			}
			if (_file < 0)
                return false;
			if (fstat(_file, &status) == -1)
			{
                close(_file);
                return false;
			}
			std::string header;
			buildHeader(request, header ,status.st_size);
			send(_fd, header.c_str(), header.size(), MSG_NOSIGNAL);
			_initialized = true;
		}
		while (nb_char)
		{
			nb_char = read(_file, buffer, 1024);
			if (nb_char > 0)
				send(_fd, buffer, nb_char, MSG_NOSIGNAL);
		}
		close(_file);
		close(_fd);
	}
	return true;
}