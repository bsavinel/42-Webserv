#include "Error.hpp"
#include <sstream>

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