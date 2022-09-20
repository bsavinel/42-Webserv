#include "HttpRequest.hpp"
#include "exceptWebserv.hpp"
#include <algorithm>
#include <iostream>

HttpRequest::HttpRequest()
{
}

HttpRequest::HttpRequest(char *client_request)
{
	int bite;
}

HttpRequest::HttpRequest(const HttpRequest & src)
{
	*this = src;
}

HttpRequest & HttpRequest::operator=(const HttpRequest & rhs)
{
	if(this != &rhs)
	{

	}
	return (*this);
}

HttpRequest::~HttpRequest()
{
}

void	HttpRequest::parseStartLine(std::string const & client_request)
{
	std::string request(client_request);
	int i;

	i = request.find(' ');
	_method.insert(0, request, 0, i - 1);
	request.erase(0, i);

	i = request.find(' ');
	_url.insert(0, request, 0, i - 1);
	request.erase(0, i);

	i = request.find('\n');
	_httpVersion.insert(0, request, 0, i - 1);
	request.erase(0, i);
}



void	HttpRequest::parseHeader(std::string &header, std::string &optionToFind, std::string &option)
{
	int i = header.find("\n" + optionToFind + ": ");
	if (i == header.npos)
		throw exceptWebserv("HttpRequest: " + optionToFind + "not found");
	option
}

void	HttpRequest::parser(char *client_request)
{
	std::string request(client_request);
	int i;

	parseStartLine(request);
	request.erase(0, request.find('\n'));
	parseHeader();
/*
	parseHeader();
	_startLine;
	_method; // GET POST DELETE
	_url; 
	_httpVersion; // Normalement on s'en branle
	_Connection;
	_Accept;
	_SecFetchSite;
	_SecFetchMode;
	_SecFetchDest;
	_Referer;
	_AcceptEncoding;
	_dnt;	
*/
}
