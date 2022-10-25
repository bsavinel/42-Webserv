#ifndef UTILS_HPP
#define UTILS_HPP

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <algorithm>

#include "exceptWebserv.hpp"

#define BUFFER_SIZE 200

std::string					read_file(char *config_file);
void						remove_comment(std::string & content_file);
void						remove_nl(std::string & content_file);
std::vector<std::string>	split_vector(std::string str, std::string delimiter);
int							checkbrackets(std::vector<std::string>::iterator it, std::vector<std::string> & splitted);
bool						is_dir_path(std::string path);
bool						is_file_path(std::string path);
std::string					get_file_extension(std::string path);
char						*ft_itoa(int n);



#endif