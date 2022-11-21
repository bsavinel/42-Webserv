#include "exceptWebserv.hpp"

exceptWebserv::exceptWebserv(const std::string content) throw()
{
	this->_content = content;
}

exceptWebserv::~exceptWebserv() throw()
{
	
}

const char *exceptWebserv::what() const throw()
{
	return this->_content.c_str();
}