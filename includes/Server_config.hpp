#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include "iostream"
#include "string"
#include "string.h"
#include "stdlib.h"
#include <utility>
#include "Location.hpp"


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

		void	setConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
		void	printConfig();
		bool	is_path_stored_yet(std::string path);

		const int&	getListeningPort() const;
		const std::string&	getServerName() const;
		const int&	getErrorCode() const;
		const std::string&	getErrorPath() const;
		const int&	getClientMaxBodySize() const;
		std::map<std::string, Location*> getLocationsMap() const;

};

#endif
