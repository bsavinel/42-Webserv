#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <iostream>
#include <list>

#include "utils.hpp"
#include "Server.hpp"

class Config
{
	private:
		std::list<Server*> servers;



	public:
		Config(char *config_file);
		Config(const Config & src);
		Config & operator=(const Config & rhs);
		~Config();


				// ----------------------
				// |		GETTERS		|
				// ----------------------

				std::list<Server*> &	getServersList();


				// ----------------------
				// |		PRINTS		|
				// ----------------------

				void					print_all_conf();

	private :
		bool					checkLocBlock();

};


#endif