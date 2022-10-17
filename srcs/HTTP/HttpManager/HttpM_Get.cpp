#include "HttpManager.hpp"
#include "autoIndex.hpp"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>
#include <iostream>

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

std::string HttpManager::LocalPathFile_get()
{
	std::string name_file;

	name_file = _request.getLocation()->getRootPath();
	name_file.erase(--name_file.end());
	name_file += _request.getUrl().first;
	if ('/' == *(--name_file.end()))
		name_file += _request.getLocation()->getIndexPath();
	std::cout << "name file :" << name_file << std::endl;
	return name_file;
}

void	HttpManager::OpenFile_get(std::string &file_name)
{
	// TODO check les droits
	_file = open(file_name.c_str(), O_RDONLY);
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
	if (nb_char > 0)
		_respond.insert(_respond.size(), &buffer[0], nb_char);
	if (nb_char < LEN_TO_READ)
		_isEnd = true;
}

void HttpManager::getMethod()
{
	std::string header;

	canWrite();
	if (_headerBuild == false)
	{
		if (!tryToGetFolder(_request.getUrl().first))
			initialize_get();
		_headerBuild = true;
	}
	else
	{
		if (tryToGetFolder(_request.getUrl().first))
		{
			_respond = autoIndex(_request);
			header = buildHeader(_respond.size(), 200);
			_respond = header + _respond;
			_isEnd = true;
		}
		else
			builRespondGet();
	}
	if (_isEnd == true)
		close(_file);
}
