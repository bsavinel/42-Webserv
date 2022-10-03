#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include "iostream"
#include "string"
#include "string.h"
#include "stdlib.h"

#include "Location_block.hpp"


class Server_config
{
	private :
		int									listening_port;
		std::string							server_name;
		int									error_code;
		std::string							error_path;
		int									client_max_body_size;
		std::map<std::string, Location*>	locations;

	public :
		Server_config();
		Server_config(const Server_config & src);
		Server_config & operator=(const Server_config & rhs);
		~Server_config();

		void	getConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
		void	printConfig();
		
		Server_config						*next;
};

#endif
