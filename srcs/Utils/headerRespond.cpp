#include "Error.hpp"
#include <string>
#include <sstream>
#include <iostream>

std::string HeaderRespond(off_t contentLenght, int statusCode, std::string type = std::string())
{
	std::stringstream ss;
	std::string	header;
	Error err;

	ss << statusCode;
	header += "HTTP/1.1 " + ss.str() + " " + err.getError(statusCode) + "\n";
	if (type.size() > 0)
		header += "Content-Type: " + type + "\n";
	if (contentLenght > 0)
	{
		ss.str("");
		ss << contentLenght;
		header += "Content-Length: " + ss.str() + "\n";
	}
	header += "\n";
	return header;
}