/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:37 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:43:38 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"
#include "autoIndex.hpp"
#include <iostream>
#include <dirent.h>

std::string	autoIndex(HttpRequest &request, HttpManager &manager)
{
	(void)(request);

	DIR				*dir;
	struct dirent	*dirToRead;
	std::string		autoIndexHtmlPage;
	std::string		dirPath;
	std::string		fileName;

	dirPath		= buildLocalPath(request);
	dir			= opendir(dirPath.c_str());

	if (dir == NULL)
	{
		if (errno == EACCES)
			manager.setErrorCode(401);
		else if (errno == ENOENT || errno == ENOTDIR)
			manager.setErrorCode(404);
		else if (errno == EMFILE || errno == ENFILE || errno == ENOMEM)
			manager.setErrorCode(500);
		return autoIndexHtmlPage;
	}

	autoIndexHtmlPage += insertHtmlHeader("\"Auto Index\"");

	while ((dirToRead = readdir(dir)) != NULL)
	{
		fileName = dirToRead->d_name;
		if (fileName.compare(".") == 0 || (fileName.compare("..") == 0 && isRootDirectory(request.getUrl().first)))
			continue ;
		else if (fileName.compare("..") == 0 && !isRootDirectory(request.getUrl().first))
			autoIndexHtmlPage += insertHtmlReference(fileName, fileName);
		else
		{
			if (dirToRead->d_type == DT_DIR)
				fileName = getAbsolutePathDirectory(request.getUrl().first, fileName);
			autoIndexHtmlPage += insertHtmlReference(fileName, fileName);
		}
	}
	autoIndexHtmlPage += insertHtmlClosing();
	return autoIndexHtmlPage;
}

std::string constructPatchFromLocationBlock(Location const * location, std::string const & fileName)
{
	std::string pathDirectory;
	std::string rootPath(location->getRootPath());

	rootPath.erase(rootPath.size() - 1, 1);
	pathDirectory += rootPath;
	pathDirectory += fileName;

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

std::string insertHtmlReference(std::string link, std::string userText)
{
	std::string href;

	href += "<a href=\"";
	href += link;
	href += "\">";
	href += userText;
	href += "</a>";
	href += "</br>";
	href += "\n";

	return href;
}

std::string insertHtmlClosing(void)
{
	std::string closingHeader;

	closingHeader += "</body>";
	closingHeader += "\n";
	closingHeader += "</html>";
	closingHeader += "\n";

	return closingHeader;
}

std::string getParentFolderPath(std::string const & folderPath)
{
	std::string		parentFolder(folderPath);

	if (*(parentFolder.rbegin()) == '/')
		parentFolder.erase(parentFolder.length() - 1, 1);
	parentFolder.erase(parentFolder.find_last_of('/') + 1, parentFolder.npos);
	return (parentFolder);
}

std::string getAbsolutePathDirectory(std::string const & folderPath, std::string & fileName)
{
	std::string pathDirectory;

	pathDirectory += folderPath;
	pathDirectory += fileName;
	pathDirectory.append("/");

	return pathDirectory;
}

int	tryToGetFolder(std::string url)
{
	if (*(url.rbegin()) == '/')
		return true;
	return false;
}

int	isRootDirectory(std::string fileName)
{
	if (fileName.compare("/") == 0)
		return true;
	return false;
}
