#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
	std::cout << "HttpRequest default constructor called" << std::endl;
}

HttpRequest::HttpRequest(const HttpRequest & src)
{
	*this = src;
	std::cout << "HttpRequest copy constructor called" << std::endl;
}

HttpRequest & HttpRequest::operator=(const HttpRequest & rhs)
{
	if(this != &rhs)
	{
		
	}
	std::cout << "HttpRequest assignment constructor called" << std::endl;
	return (*this);
}

HttpRequest::~HttpRequest()
{
	std::cout << "HttpRequest default destructor called" << std::endl;
}