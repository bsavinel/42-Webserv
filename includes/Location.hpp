#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>

class Location
{
	std::vector<std::string>	allowed_methods;
	int							return_code;
	std::string					redirection_path;
	std::string					root_path;
	std::string					index_path;
	bool						autoindex;
	std::string					cgi_file_extension;
	std::string					cgi_path_to_script;
	std::string					upload_dir;

	public :
		Location();
		Location(const Location & src);
		Location & operator=(const Location & rhs);
		~Location();

		void	getConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
		void	printConfig();
};

#endif