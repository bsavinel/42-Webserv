#ifndef PARSER_HPP
#define PARSER_HPP

#include <exceptWebserv.hpp>
#include <string>
#include <fcntl.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <Server_config.hpp>
#include <algorithm>
#include <stdlib.h>
#include <list>

#define BUFFER_SIZE 200

class Config
{
	private:
		std::list<Server_config*> servers;
	public:
		Config();
		Config(const Config & src);
		Config & operator=(const Config & rhs);
		~Config();

		void parser(char *config_file);
		void	print_all_conf();
};

char	**ft_split(char const *s, char c);



#endif