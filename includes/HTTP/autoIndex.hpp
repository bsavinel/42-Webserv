#ifndef AUTO_INDEX_HPP
# define AUTO_INDEX_HPP

#include "HttpManager.hpp"
#include "iostream"
#include <dirent.h>

std::string	autoIndex(HttpRequest &request);
int	tryToGetFolder(std::string url);
std::string getParentFolderPath(std::string const & folderPath);
std::string constructPatchFromLocationBlock(Location const * location, std::string const & fileName);
std::string getAbsolutePathDirectory(std::string const & folderPath, std::string & fileName);
std::string insertHtmlHeader(std::string title);
std::string insertHtmlClosingHeader(void);
std::string insertHtmlReference(std::string link, std::string userText);

#endif