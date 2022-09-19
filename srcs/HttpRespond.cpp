#include "HttpRespond.hpp"
#include <stdlib.h>

char	*ft_itoa(int n);

HttpRespond::HttpRespond()
{
}

HttpRespond::HttpRespond(const HttpRespond& rhs)
{
	*this = rhs;
}

HttpRespond::HttpRespond(const std::string content, const std::string contentType)
{
	_content = content;
	_contentType = contentType;
}

HttpRespond::~HttpRespond()
{
}

HttpRespond &HttpRespond::operator=(const HttpRespond& rhs)
{
	this->_content = rhs._content;
	this->_contentType = rhs._contentType;
	return *this;
}

const std::string HttpRespond::getContent() const
{
	return _content;
}

std::string HttpRespond::getHeader() const
{
	return "HTTP/1.1 200 OK\nContent-Type:" + _contentType + "\nContent-Length: " + ft_itoa(_content.length())  + "\n\n";
}

std::string HttpRespond::getHttpRespond() const
{
	return(getHeader() + _content);
}

void HttpRespond::setContent(const std::string content)
{
	_content = content;
}

void HttpRespond::setContentType(const std::string contentType)
{
	_contentType = contentType;
}