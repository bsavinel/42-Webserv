#include "HttpManager.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
void	autoIndex(HttpRequest &request);

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
}

int	tryToGetFolder(std::string url)
{
	if (url.size() !=1 && *(url.rbegin()) == '/')
		return true;
	return false;
}

void	HttpManager::initialize_get()
{
	struct stat status;
	//* open good file and create header + regarder page ererue necesaire a envoye

	// TODO faire un read de 0 pour voir si on peut bien read dessus
	if(tryToGetFolder(_request.getUrl().first))
	{
		// 	_file = autoIndex(_request);
	}
	else if (_request.getUrl().first.compare("/") == 0)
	{
		_file = open("./data/www/index.html", O_RDONLY);
	}
	// else if (tryToGetFolder(_request.getUrl().first))
	// 	_file = autoIndex(_request);
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
	// std::string str(buffer);
	// std::cout << "================="<<std::endl;
	// std::cout << str << std::endl;
	// std::cout << "================="<<std::endl;
	if (nb_char > 0)
		_respond.insert(_respond.size(), &buffer[0], nb_char);
	if (nb_char < LEN_TO_READ)
		_endGet = true;
}

void	HttpManager::getMethod()
{
	if (_endGet == true)
	{
		_isEnd = true;
		return ;
	}
	canWrite();
	if (_headerBuild == false)
	{
		initialize_get();
	}
	builRespondGet();
	if (_endGet == true)
		close(_file);
}
