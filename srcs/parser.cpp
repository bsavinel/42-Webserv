#include "parser.hpp"


Config::Config()
{
}

Config::~Config()
{
	for (std::list<Server_config*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
		delete *it;
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
	close(fd);
	return (file_content);
}

void	remove_comment(std::string & content_file)
{
	std::string::iterator beg = content_file.begin();
	std::string::iterator end_of_comment;
	std::string::iterator end = content_file.end();

	while(beg != end)
	{
		if(*beg == '#')
		{
			end_of_comment = beg;
			while (*end_of_comment != '\n' && end_of_comment != end)
				end_of_comment++;
			content_file.erase(beg, end_of_comment);
		}
		beg++;
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
					if((*it).compare("location") == 0)
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

Server_config *getServerToken(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	Server_config *server = new Server_config();
	server->getConfig(it, splitted);
	return(server);
}


void Config::parser(char *config_file)
{
	std::string content_file = read_file(config_file);
	remove_comment(content_file);
	remove_nl(content_file);

	std::string delimiter(" \t;{}");
	std::vector<std::string> splitted = split_vector(content_file, delimiter);

	for(std::vector<std::string>::iterator beg = splitted.begin(); beg != splitted.end(); beg++)
	{
		if ((*beg).compare("server") == 0 && (*(beg + 1)).compare("{") == 0)
		{
			beg++;
			if(checkbrackets(beg, splitted))
				this->servers.push_back(getServerToken(beg, splitted));
		}
	}
}

void	Config::print_all_conf()
{
	int i  = 0;
	if(this->servers.empty())
		throw exceptWebserv("Error config : no congiguration found");
	for(std::list<Server_config*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{	std::cout << "------CONFIG " << i <<"------" << std::endl;
		(*it)->printConfig();
		i++;
	}
	
}