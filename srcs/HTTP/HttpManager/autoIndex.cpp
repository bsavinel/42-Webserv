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
			dir = opendir("./data/www");
	//	}
//	else
//	{
//		std::string path = "." + request.getLocation()->getRootPath();
//		dir = opendir(path.c_str());
//	}
//	dirToRead = readdir(dir);
	index += "<!DOCTYPE html>";
	index += "<html lang=\"en\">";
	index += "<html>";
	index += "<head><title> index </title></head>";
	index += "<body>";
	while ((dirToRead = readdir(dir)) != NULL)
	{
		std::string file(dirToRead->d_name);
		index += "<a href=\"";
		index += file;
		index += "\"></a>";
	}
	index += "</body>";
	index += "</html>";
	std::cout << "index: " << index << std::endl;
}

