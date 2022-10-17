#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <stdlib.h>
#include <ctype.h>

#include <string>
#include <iostream>
#include <vector>
#include <map>

class Location
{
	private:
		std::vector<std::string>	allowed_methods;
		int							return_code;
		std::string					redirection_path;
		std::string					root_path;
		std::string					index_path;
		bool						autoindex;
		std::string					cgi_file_extension;
		std::string					cgi_path_to_script;
		std::string					upload_dir;
		std::string					locate;

	public :
		Location();
		Location(const Location & src);
		Location & operator=(const Location & rhs);
		~Location();

		void	setConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted, std::string &loc);
		void	printConfig();

		std::vector<std::string>	getAllowedMethods() const;
		const int&					getReturnCode() const;
		const std::string&			getRedirectionPath() const;
		const std::string&			getRootPath() const;
		const std::string&			getIndexPath() const;
		const bool&					getAutoPath() const;
		const std::string&			getCgiFileExtension() const;
		const std::string&			getCgiPathToScript() const;
		const std::string&			getUploadDirectory() const;
		const std::string&			getLocate() const;
};

#endif