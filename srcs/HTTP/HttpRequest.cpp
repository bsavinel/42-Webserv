#include "HttpRequest.hpp"
#include "exceptWebserv.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include <algorithm>
#include <iostream>

#include <fstream>
#include <string>
#include <iostream>

HttpRequest::HttpRequest()
{
}

HttpRequest::HttpRequest(std::string client_request)
{
	_request = client_request;
}

HttpRequest::HttpRequest(const HttpRequest & src)
{
	*this = src;
}

HttpRequest & HttpRequest::operator=(const HttpRequest & rhs)
{
	if(this != &rhs)
	{}
	return (*this);
}

HttpRequest::~HttpRequest()
{

}
 
// std::vector<std::string> splitUrl(std::string url, std::string del)
// {
// 	std::vector<std::string> split;

//     int start, end = -1*del.size();
//     do {
//         start = end + del.size();
//         end = url.find(del, start);
//         split.push_back(url.substr(start, end - start));
//     } while (end != -1);
// 	return (split);
// }

// int sizeSplitUrl(std::string url, std::string del)
// {
// 	std::vector<std::string> split;
// 	int							splitSize = 0;
//     int start, end = -1*del.size();
//     do {
//         start = end + del.size();
//         end = url.find(del, start);
//         split.push_back(url.substr(start, end - start));
//     } while (end != -1);
// 	return (split.size());
// }

Location	*HttpRequest::findLocation(const Server &server)
{
	std::map<std::string, Location*>			tmpMapLocation			= server.getLocationsMap();
	std::map<std::string, Location*>::iterator	itLocationBlock			= tmpMapLocation.begin();
	std::map<std::string, Location*>::iterator	endIttLocationBlock		= tmpMapLocation.end();
	std::string									urlCopy					= _url.first;

	while (!urlCopy.empty())
	{
		itLocationBlock = tmpMapLocation.begin();
		while (itLocationBlock != endIttLocationBlock)
		{
			if (itLocationBlock->first.compare(urlCopy.c_str()) == 0)
				return itLocationBlock->second;
			itLocationBlock++;
		}
		if (urlCopy.find_last_of("/") != urlCopy.npos)
		{
			urlCopy.erase(urlCopy.find_last_of("/") , urlCopy.size());
			urlCopy.erase(urlCopy.find_last_of("/") + 1 , urlCopy.size());
		}
		else
			urlCopy.clear();
	}
	return (server.getLocationsMap().find("/")->second);
}

void	HttpRequest::parseStartLine(std::string const & client_request)
{
	std::string request(client_request);
	int i;

	i = request.find(' ');
	_method.first.insert(0, request, 0, i);
	request.erase(0, i + 1);

	i = request.find(' ');
	_url.first.insert(0, request, 0, i);
	request.erase(0, i + 1);

	i = request.find('\n');
	_httpVersion.first.insert(0, request, 0, i);
	request.erase(0, i + 1);
}

std::pair <std::string, bool>	HttpRequest::parseHeader(std::string &header, std::string optionToFind)
{
	std::pair <std::string, bool> option;

	size_t optionToFindPos = header.find(optionToFind);
	size_t optionSize = optionToFind.length();
	size_t copyStart = optionToFindPos + optionSize;
	size_t copyUntil = header.find("\n", copyStart) -1;

	if (optionToFindPos == header.npos)
	{
//		std::cout << "HTTP request option not found" << std::endl << optionToFindPos << std::endl;
		option.second = false;
	}
	else
	{
		option.first = header.substr(copyStart, copyUntil - copyStart);
		option.second = true;
	}
	return (option);
}

void	HttpRequest::parser(/*std::string &request*/)
{
	parseStartLine(_request);
	_request.erase(0, _request.find('\n') + 1);

	_Connection = parseHeader(_request, "\nConnection: ");
	_Accept = parseHeader(_request, "\nAccept: ");
	_SecFetchSite = parseHeader(_request, "\nSec-Fetch-Site: ");
	_SecFetchMode = parseHeader(_request, "\nSec-Fetch-Mode: ");
	_SecFetchDest = parseHeader(_request, "\nSec-Fetch-Dest: ");
	_Referer = parseHeader(_request, "\nReferer: ");
	_AcceptEncoding = parseHeader(_request, "\nAccept-Encoding: ");

	_request.erase(0, _request.find("\r\n\r\n") + 1);
}

void	HttpRequest::concatenate(char *str)
{
	_request.append(str);
}

void	HttpRequest::erase(int index)
{
	_request.erase(0, index);
}

void HttpRequest::setRequest(std::string const & request)
{
	_request = request;
} 

void HttpRequest::setLocation(Location *location)
{
	_location = location;
}

std::string	HttpRequest::getRequest(void) const
{
	return _request;
}

std::pair<std::string, bool> HttpRequest::getMethod(void) const
{
	return _method;
} 

std::pair<std::string, bool> HttpRequest::getUrl(void) const
{
	return _url;
} 
std::pair<std::string, bool> HttpRequest::getHttpVersion(void) const
{
	return _httpVersion;
}
std::pair<std::string, bool> HttpRequest::getConnection(void) const
{
	return _Connection;
}
std::pair<std::string, bool> HttpRequest::getAccept(void) const
{
	return _Accept;
}
std::pair<std::string, bool> HttpRequest::getSecFetchSite(void) const
{
	return _SecFetchSite;
}
std::pair<std::string, bool> HttpRequest::getSecFetchMode(void) const
{
	return _SecFetchMode;
}
std::pair<std::string, bool> HttpRequest::getSecFetchDest(void) const
{
	return _SecFetchDest;
}
std::pair<std::string, bool> HttpRequest::getReferer(void) const
{
	return _Referer;
}
std::pair<std::string, bool> HttpRequest::getAcceptEncoding(void) const
{
	return _AcceptEncoding;
}
std::pair<std::string, bool> HttpRequest::getDnt(void) const
{
	return _dnt;
}

Location					*HttpRequest::getLocation(void) const
{
	return _location;
}

std::ostream &	operator<<( std::ostream & o, HttpRequest const & rhs)
{
	o << rhs.getMethod().first << std::endl;
	o << rhs.getUrl().first << std::endl; 
	o << rhs.getHttpVersion().first << std::endl;
	o << rhs.getConnection().first << std::endl;
	o << rhs.getAccept().first << std::endl;
	o << rhs.getSecFetchSite().first << std::endl;
	o << rhs.getSecFetchMode().first << std::endl;
	o << rhs.getSecFetchDest().first << std::endl;
	o << rhs.getReferer().first << std::endl;
	o << rhs.getAcceptEncoding().first << std::endl;
	o << rhs.getDnt().first << std::endl;
	return o;
}
