#include "HttpManager.hpp"
#include "iostream"
#include <dirent.h>

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

std::string getParentFolderPath(std::string const & folderPath)
{
	std::string		parentFolder(folderPath);

	if (*(parentFolder.rbegin()) == '/')
		parentFolder.erase(parentFolder.length() - 1, 1);
	parentFolder.erase(parentFolder.find_last_of('/') + 1, parentFolder.npos);
	return (parentFolder);
}

std::string constructPatchFromLocationBlock(Location const * location, std::string const & fileName)
{
	std::string pathDirectory;

	pathDirectory += location->getRootPath();
	pathDirectory += fileName;
	pathDirectory.append("/");

	return pathDirectory;
}

std::string getAbsolutePathDirectory(std::string const & folderPath, std::string & fileName)
{
	std::string pathDirectory;

	pathDirectory += folderPath;
	pathDirectory += fileName;
	pathDirectory.append("/");

	return pathDirectory;
}

std::string insertHtmlHeader(std::string title)
{
	std::string header;

	header += "<!DOCTYPE html>";
	header += "\n";
	header += "<html lang=\"en\">";
	header += "\n";
	header += "<html>";
	header += "\n";
	header += "<head><title>";
	header += title;
	header += "</title></head>";
	header += "\n";
	header += "<body>";
	header += "\n";	

	return header;
}

std::string insertHtmlClosingHeader(void)
{
	std::string closingHeader;

	closingHeader += "</body>";
	closingHeader += "\n";
	closingHeader += "</html>";
	closingHeader += "\n";

	return closingHeader;
}

std::string insertHtmlReference(std::string link, std::string userText)
{
	std::string href;

	href += "<a href=\"";
	href += link;
	href += "\">";
	href += userText;
	href += "</a>";
	href += "\n";

//	std::cout << "href = [" << href << "]" << std::endl;
	return href;
}

void	autoIndex(HttpRequest &request)
{
	(void)(request);

	DIR				*dir;
	struct dirent	*dirToRead;
	std::string		parentFolder;
	std::string		index;
	std::string		url;
	std::string		fileName;

/*TEMPORAIRE*/

//	url				= constructPatchFromLocationBlock(request.getLocation(), request.getUrl().first);
	std::cout << "url = " << url << std::endl;
	parentFolder	= getParentFolderPath(url);
	dir				= opendir(url.c_str());

//	std::string url("./data/www/");
//	dir = opendir(url.c_str());
//	std::string		parentFolder = getParentFolderPath(url);
/*FIN TEMPORAIRE*/

	index += insertHtmlHeader("\"Auto Index\"");

	while ((dirToRead = readdir(dir)) != NULL)
	{
		fileName = dirToRead->d_name;
//		std::cout << "fileName : " << fileName << std::endl;
		if (fileName.compare(".") == 0)
			continue ;
		else if (fileName.compare("..") == 0)
			index += insertHtmlReference(parentFolder, parentFolder);
		else
		{
			if (dirToRead->d_type == DT_DIR)
				fileName = getAbsolutePathDirectory(url, fileName);
			index += insertHtmlReference(fileName, fileName);
		}
	}

	index += insertHtmlClosingHeader();

	std::cout << std::endl << "START index: "  << std::endl<< std::endl << index << std::endl;

	std::cout << "END index" << std::endl;
}
