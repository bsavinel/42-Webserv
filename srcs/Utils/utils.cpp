/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:21 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 08:55:58 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

int	checkbrackets(std::vector<std::string>::iterator it, std::vector<std::string> & splitted)
{
	while (it != splitted.end() && (*it).compare("}") != 0)
	{
		if((*it).compare("server") == 0)
			throw exceptWebserv("Config Error : Server block nested into another server block");
		if ((*it).compare("location") == 0)
		{
			if ((it + 2) < splitted.end() && (*(it + 2)).compare("{") == 0)
			{
				while (it != splitted.end() && (*it).compare("}") != 0 )
				{
					it++;
					if(it == splitted.end() || (*it).compare("location") == 0)
						throw exceptWebserv("Config Error : Location block not closed");
				}
				if (it == splitted.end())
					throw exceptWebserv("Config Error : Location block not closed");
			}
			else 
				throw exceptWebserv("Config Error : location block format not corresponding");
		}
		it++;
	}
	if (it == splitted.end())
		throw exceptWebserv("Config Error : Server block not closed");
	return (true);
}

std::string read_file(char *config_file)
{
	int		fd;
	char	buff[BUFFER_SIZE + 1];
	int		ret;
	
	std::string file_content;
	file_content.clear();

	memset(buff,0, BUFFER_SIZE + 1);
	fd = open(config_file, O_RDONLY);
	if(fd < 0)
		throw exceptWebserv("Config Error : can't open the file");
	ret = read(fd, buff, BUFFER_SIZE);
	if(ret == -1)
		throw exceptWebserv("Config Error : something went wrong while reading the file");
	while(ret > 0)
	{
		buff[ret] = 0;
		file_content.append(buff);
		memset(buff,0, BUFFER_SIZE+1);
		ret = read(fd, buff, BUFFER_SIZE);
	}
	if(ret == -1)
		throw exceptWebserv("Config Error : something went wrong while reading the file");
	close(fd);
	return (file_content);
}

			#include <iostream>
void	remove_comment(std::string & content_file)
{
	std::string::iterator beg = content_file.begin();
	std::string::iterator end_of_comment;
	std::string::iterator end = content_file.end();

	while(beg < end)
	{
		if(*beg == '#')
		{
			end_of_comment = beg;
			while (end_of_comment != end && *end_of_comment != '\n')
				end_of_comment++;
			content_file.erase(beg, end_of_comment);
		}
		beg++;
		end = content_file.end();
	}
}

void	remove_nl(std::string & content_file)
{
	content_file.erase(std::remove(content_file.begin(), content_file.end(), '\n'), content_file.end());
}

std::vector<std::string> split_vector(std::string str, std::string delimiter)
{
	std::vector<std::string>	splitted;
	std::string::iterator		beg = str.begin();

	for(std::string::iterator stop = beg; beg != str.end(); beg = stop)
	{
		stop = beg;
		size_t del = delimiter.find(*stop, 0);
		while(del == std::string::npos && stop++ != str.end())
			del = delimiter.find(*stop, 0);
		if(stop != beg)
		{
			std::string word(beg, stop);
			splitted.push_back(word);
		}
		if(del != std::string::npos)
		{
			if (del > 1)
			{
				std::string word;
				word += delimiter[del];
				splitted.push_back(word);
			}
			stop++;
		}
	}
	return(splitted);
}

bool	is_dir_path(std::string path)
{
	if(path.rfind("/", 0) == std::string::npos && path.rfind("./", 0) == std::string::npos)
		return (0);
	std::string::iterator lastchar= path.end() - 1;
	if((*lastchar) != '/')
		return(0);
	else
		return(1);
}

bool	is_file_path(std::string path)
{
	if(path.rfind("/", 0) == std::string::npos && path.rfind("./", 0) == std::string::npos)
		return (0);
	std::string::iterator lastchar= path.end() - 1;
	if(!isalnum(*lastchar))
		return(0);
	return (1);
}

bool	is_extension_file(std::string ext)
{
	std::string::iterator it = ext.begin();
	if(*it != '.')
		return (false);
	it++;
	while (it != ext.end())
	{
		if(!isalnum(*it))
			return (false);
		it++;
	}
	return(true);
}

std::string get_file_extension(std::string path)
{
	std::string extension;
			
	if(path.rfind('.') != std::string::npos)
	{
		std::size_t start = path.rfind('.');
		std::size_t nbr_to_cpy = path.size() - start;

		extension = path.substr(start, nbr_to_cpy);
	}

	return(extension);
}

std::string retrieve_from_left_till_char(std::string path, char c)
{
	std::string arg;
			
	if(path.find_last_of(c) != std::string::npos)
	{
		std::size_t start = path.find_last_of(c) + 1;
		std::size_t nbr_to_cpy = path.size() - start;

		arg = path.substr(start, nbr_to_cpy);
	}
	return(arg);
}

bool	check_existing_error_code(int nbr)
{
	std::list<int> error_code;
	error_code.push_back(100);
	error_code.push_back(101);
	error_code.push_back(200);
	error_code.push_back(201);
	error_code.push_back(202);
	error_code.push_back(203);
	error_code.push_back(204);
	error_code.push_back(205);
	error_code.push_back(206);
	error_code.push_back(300);
	error_code.push_back(301);
	error_code.push_back(302);
	error_code.push_back(303);
	error_code.push_back(304);
	error_code.push_back(305);
	error_code.push_back(306);
	error_code.push_back(307);
	error_code.push_back(400);
	error_code.push_back(401);
	error_code.push_back(402);
	error_code.push_back(403);
	error_code.push_back(404);
	error_code.push_back(405);
	error_code.push_back(406);
	error_code.push_back(407);
	error_code.push_back(408);
	error_code.push_back(409);
	error_code.push_back(410);
	error_code.push_back(411);
	error_code.push_back(412);
	error_code.push_back(413);
	error_code.push_back(414);
	error_code.push_back(415);
	error_code.push_back(416);
	error_code.push_back(417);
	error_code.push_back(500);
	error_code.push_back(501);
	error_code.push_back(502);
	error_code.push_back(503);
	error_code.push_back(504);
	error_code.push_back(505);

	std::list<int>::iterator it = error_code.begin();
	std::list<int>::iterator ite = error_code.end();

	while (it != ite)
	{
		if(*it == nbr)
			return (1);
		it++;
	}
	return (0);
	
}