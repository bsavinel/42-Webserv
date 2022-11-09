#include "Error.hpp"
#include <sstream>
#include <sys/time.h>
#include "HttpRequest.hpp"

std::string buildErrorPage(int err)
{
	std::string page;
	Error allErr;

	std::stringstream ss;
	ss << err;

	page += "<!DOCTYPE html>\n";
	page += "<html lang=\"en\">\n";
	page += "<html>\n";
	page += "<body>\n";
	page += "<h1 style=\"text-align:center\">\n";
	page += ss.str();
	page += "\n</h1>\n";
	page += "<hr>\n";
	page += "<p style=\"text-align:center\">\n";
	page += allErr.getError(err);
	page += "\n</p>\n";
	page += "</body>\n";
	return page;
}

std::string buildLocalPath(HttpRequest &request)
{
	std::string	localPath;
	const std::string &locationPath = request.getLocation()->getLocate();
	const std::string &UrlPath = request.getUrl().first;
	const std::string &RootPath = request.getLocation()->getRootPath();

	localPath.insert(0, UrlPath, locationPath.size(), UrlPath.size() - locationPath.size());
	if (localPath[0] == '/')
		localPath.erase(0, 1);
	localPath.insert(0, RootPath);
	return localPath;
}

size_t	give_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((size_t)time.tv_sec);
}