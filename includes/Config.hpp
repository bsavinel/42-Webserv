#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <iostream>
#include <list>

#include <utils.hpp>
#include "Server_config.hpp"

class Config
{
	private:
		std::list<Server_config*> servers;

	public:
		Config(char *config_file);
		Config(const Config & src);
		Config & operator=(const Config & rhs);
		~Config();

		void	print_all_conf();
};


#endif