#include "HttpManager.hpp"
#include "iostream"
#include <dirent.h>

void	autoIndex(HttpRequest &request)
{
	(void)(request);
	DIR				*dir;
	struct dirent	*dirToRead;
	std::string		index;

//	dir = opendir(request.getLocation()->getRootPath().c_str());
//	std::cout << "PATH = [" << request.getLocation()->getRootPath() << "]" << std::endl;
	//	if (request.getLocation()->getRootPath().compare("/") == 0)
	//	{
			dir = opendir(".");
	//	}
//	else
//	{
//		std::string path = "." + request.getLocation()->getRootPath();
//		dir = opendir(path.c_str());
//	}
//	dirToRead = readdir(dir);
	index += "<html>";
	index += "<head><title> index </title></head>";
	index += "<body>";
	while ((dirToRead = readdir(dir)) != NULL)
	{
		std::cout << dirToRead->d_name << std::endl;
	}
	index += "</body>";
	index += "</html>";
}
