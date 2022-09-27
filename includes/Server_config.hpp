#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

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
		Server_config(char *config_file);
		Server_config(const Server_config & src);
		Server_config & operator=(const Server_config & rhs);
		~Server_config();
};

#endif
