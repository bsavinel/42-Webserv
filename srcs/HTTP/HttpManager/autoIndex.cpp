#include "HttpManager.hpp"
#include "iostream"
#include <dirent.h>

std::string getParentFolderPath(std::string const & folderPath)
{
	std::string		parentFolder(folderPath);

	if (*(parentFolder.rbegin()) == '/')
		parentFolder.erase(parentFolder.length() - 1, 1);
	parentFolder.erase(parentFolder.find_last_of('/') + 1, parentFolder.npos);
	return (parentFolder);
}

void	autoIndex(HttpRequest &request)
{
	(void)(request);
	DIR				*dir;
	struct dirent	*dirToRead;
	std::string		index;

//	std::string		parentFolder = getParentFolderPath(request.getUrl().first);
//	dir = opendir(request.getUrl().first.c_str());

	std::string url("./data/www/");
	dir = opendir(url.c_str());
	std::string		parentFolder = getParentFolderPath(url);

	index += "<!DOCTYPE html>";
	index += "\n";
	index += "<html lang=\"en\">";
	index += "\n";
	index += "<html>";
	index += "\n";
	index += "<head><title> index </title></head>";
	index += "\n";
	index += "<body>";
	index += "\n";
	while ((dirToRead = readdir(dir)) != NULL)
	{
		std::string file(dirToRead->d_name);
		std::cout << "file : " << file << std::endl;
		if (file.compare(".") == 0)
			continue ;
		else if (file.compare("..") == 0)
		{
			index += "<a href=\"";
			index += parentFolder;
			index += "\"></a>";
			index += "\n";
		}
		else
		{
			index += "<a href=\"";
			index += file;
			index += "\"></a>";
			index += "\n";
		}
	}
	index += "</body>";
	index += "\n";
	index += "</html>";
	index += "\n";
	std::cout << "index: " << std::endl << index << std::endl;

	std::cout << ": index" << std::endl;
}
