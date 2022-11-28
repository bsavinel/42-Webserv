/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:45:10 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:45:11 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <list>
#include <algorithm>

#include "exceptWebserv.hpp"

#define BUFFER_SIZE 10000

std::string					read_file(char *config_file);
void						remove_comment(std::string & content_file);
void						remove_nl(std::string & content_file);
std::vector<std::string>	split_vector(std::string str, std::string delimiter);
int							checkbrackets(std::vector<std::string>::iterator it, std::vector<std::string> & splitted);
bool						is_dir_path(std::string path);
bool						is_file_path(std::string path);
bool						is_extension_file(std::string ext);
std::string					get_file_extension(std::string path);
char						*ft_itoa(int n);
std::string					retrieve_from_left_till_char(std::string path, char c);
bool						check_existing_error_code(int nbr);
size_t						give_time(void);

#endif