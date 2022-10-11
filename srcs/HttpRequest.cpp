#include "HttpRequest.hpp"
#include "exceptWebserv.hpp"
#include <algorithm>
#include <iostream>

#include <fstream>
#include <string>
#include <iostream>

void	HttpRequest::parser(/*std::string &request*/)
{
	parseStartLine(_request);
	_request.erase(0, _request.find('\n') + 1);

	std::cout << _request << std::endl;

	_Connection = parseHeader(_request, "\nConnection: ");
	_Accept = parseHeader(_request, "\nAccept: ");
	_SecFetchSite = parseHeader(_request, "\nSec-Fetch-Site: ");
	_SecFetchMode = parseHeader(_request, "\nSec-Fetch-Mode: ");
	_SecFetchDest = parseHeader(_request, "\nSec-Fetch-Dest: ");
	_Referer = parseHeader(_request, "\nReferer: ");
	_AcceptEncoding = parseHeader(_request, "\nAccept-Encoding: ");
	_Boundary = parseHeader(_request, " boundary=");
	_ContentType = parseHeader(_request, "\nContent-Type: ");

	_request.erase(0, _request.find("\r\n\r\n") + 1);
	 std::cout << "    ====== " << "BODY START" << " ======    "  << std::endl;
	std::cout << _request << std::endl;
	 std::cout << "    ====== " << "BODY END" << " ======    "  << std::endl;
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


void	HttpRequest::concatenate(char *str)
{
	_request.append(str);
}

void	HttpRequest::erase(int index)
{
	_request.erase(0, index);
}

std::ostream &	operator<<( std::ostream & o, HttpRequest const & rhs)
{
	if (rhs.getMethod().second)
		o << rhs.getMethod().first << std::endl;
	if (rhs.getUrl().second)
		o << rhs.getUrl().first << std::endl; 
	if (rhs.getHttpVersion().second)
		o << rhs.getHttpVersion().first << std::endl;
	if (rhs.getConnection().second)
		o << rhs.getConnection().first << std::endl;
	if (rhs.getAccept().second)
		o << rhs.getAccept().first << std::endl;
	if (rhs.getSecFetchSite().second)
		o << rhs.getSecFetchSite().first << std::endl;
	if (rhs.getSecFetchMode().second)
		o << rhs.getSecFetchMode().first << std::endl;
	if (rhs.getSecFetchDest().second)
		o << rhs.getSecFetchDest().first << std::endl;
	if (rhs.getReferer().second)
		o << rhs.getReferer().first << std::endl;
	if (rhs.getAcceptEncoding().second)
		o << rhs.getAcceptEncoding().first << std::endl;
	if (rhs.getDnt().second)
		o << rhs.getDnt().first << std::endl;
	if (rhs.getContentType().second)
		o << rhs.getContentType().first << std::endl;
	if (rhs.getBoundary().second)
		o << rhs.getBoundary().first << std::endl;
	return o;
}
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
void HttpRequest::setRequest(std::string const & request)
{
	_request = request;
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
std::pair<std::string, bool> HttpRequest::getContentType(void) const
{
	return _ContentType;
}

std::pair<std::string, bool> HttpRequest::getBoundary(void) const
{
	return _Boundary;
}
