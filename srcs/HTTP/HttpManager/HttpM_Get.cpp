#include "HttpManager.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
void	autoIndex(HttpRequest &request);
std::string HttpManager::buildHeader(off_t contentLenght, int statusCode)
{
	std::string	header;

	(void)statusCode;
	header += "HTTP/1.1 200 OK\n"; // TODO voir les image apres
	if (_request.getUrl().first.find("html") != std::string::npos)
		header += "Content-Type: text/html\n";
	else if (_request.getUrl().first.find("css") != std::string::npos)
		header += "Content-Type: text/css\n";
	else if (_request.getUrl().first.find("ico") != std::string::npos)
		header += "Content-Type: image/x-icon\n";
	if (_request.getUrl().first.compare("/") == 0)
		header += "Content-Type: text/html\n";

	if (contentLenght > 0)
	{
		std::stringstream ss;
		ss << contentLenght;
		header += "Content-Length: " + ss.str() + "\n";
	}
	header += "\n";
	return header;
}

void	HttpManager::initialize_get()
{
	std::string name_file;

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
}

void	HttpManager::initialize_get()
{
	struct stat status;

	if (1) // ? condition autoindex
	{
		_name_file = LocalPathFile_get();
		OpenFile_get(_name_file);
		stat(_name_file.c_str(), &status);
		_respond = buildHeader(status.st_size, 200);
	}
	_headerBuild = true;
}

void HttpManager::builRespondGet()
{
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	nb_char = read(_file, buffer, LEN_TO_READ);
	// std::string str(buffer);
	// std::cout << "================="<<std::endl;
	// std::cout << str << std::endl;
	// std::cout << "================="<<std::endl;
	if (nb_char > 0)
		_respond.insert(_respond.size(), &buffer[0], nb_char);
	if (nb_char < LEN_TO_READ)
		_isEnd = true;
}

void HttpManager::getMethod()
{
	canWrite();
	/*if (tryToGetFolder(_request.getUrl().first))
	{
		autoindex
		_headerBuild == true;
	}
	else */if (_headerBuild == false)
		initialize_get();
	else
		builRespondGet();
	if (_isEnd == true)
		close(_file);
}
