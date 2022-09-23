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
#define WHITESPACE " \n\r\t\f\v"


// typedef struct s_config
// {
// 	Server_config	*server_config;
// 	s_config		*next;
// }	t_config;

void parser(char *config_file_path);
std::string read_file(char *config_file);
std::string rtrim_wsp(std::string str);
std::string ltrim_wsp(std::string str);

#endif