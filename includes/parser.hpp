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

#define BUFFER_SIZE 200

typedef struct s_config
{
	Server_config	*server_config;
	s_config		*next;
}	t_config;


void	parser(char *config_file);

#endif