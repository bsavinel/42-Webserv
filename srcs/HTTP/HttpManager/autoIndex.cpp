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

std::string getAbsolutePathDirectory(std::string const & folderPath, std::string &fileName)
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

	url 			= request.getUrl().first;
	std::cout << "url = " << url << std::endl;
	parentFolder	= getParentFolderPath(url);
	dir				= opendir(url.c_str());

/*TEMPORAIRE*/
//	std::string url("./data/www/");
//	dir = opendir(url.c_str());
//	std::string		parentFolder = getParentFolderPath(url);
/*FIN TEMPORAIRE*/

	index += insertHtmlHeader("\"Auto Index\"");

	while ((dirToRead = readdir(dir)) != NULL)
	{
		fileName = dirToRead->d_name;
		std::cout << "fileName : " << fileName << std::endl;
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
