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

#define BUFFER_SIZE 200

typedef struct s_config
{
	Server_config	*server_config;
	s_config		*next = NULL;
}	t_config;


void parser(char *config_file, t_config **head);
char	**ft_split(char const *s, char c);
void	print_all_conf(t_config *head);



#endif