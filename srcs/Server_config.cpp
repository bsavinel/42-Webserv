#include "Server_config.hpp"


Server_config::Server_config()
: listening_port(-1), error_code(0), client_max_body_size(0)
{
	// std::cout << "Server_config default constructor called" << std::endl;
}

Server_config::Server_config(const Server_config & src)
{
	*this = src;
	// std::cout << "Server_config copy constructor called" << std::endl;
}

Server_config & Server_config::operator=(const Server_config & rhs)
{
	if(this != &rhs)
	{
		
	}
	// std::cout << "Server_config assignment constructor called" << std::endl;
	return (*this);
}

Server_config::~Server_config()
{
	int i = 0;
	std::cout << "SIZE " << locations.size() << std::endl;
	;
	for(std::map<std::string, Location*>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		delete it->second;
		i++;
	}
	std::cout << "BOUCLE " << i << std::endl;
	std::cout << "Server_config default destructor called" << std::endl;
}

bool	Server_config::is_path_stored_yet(std::string path)
{
	for(std::map<std::string, Location*>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if(path == it->first)
			return(true);
	}
	return(false);
}

void Server_config::getConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	while (it != splitted.end() && (*it).compare("}") != 0)
	{
		if((*it).compare("location") == 0)
		{
			it++;
			std::string path_loc = *it;
			if(!is_path_stored_yet(path_loc))
			{
				Location *new_loc = new Location();
				new_loc->getConfig(it, splitted);
				this->locations.insert(std::make_pair(path_loc, new_loc));
			}
		}
		if ((*it).compare("listen") == 0)
		{
			it++;
			this->listening_port = atoi((*it).c_str());
			//std::cout << "listening port = " << listening_port << std::endl;
		}
		else if ((*it).compare("server_name") == 0)
		{
			it++;
			this->server_name = *it;
			//std::cout << "server_name = " << server_name << std::endl;
		}
		else if ((*it).compare("error_pages") == 0)
		{
			it++;
			this->error_code = atoi((*it).c_str());
			//std::cout << "error_pages = " << error_code << std::endl;
			it++;
			this->error_path = *it;
			//std::cout << "error_pages = " << error_path << std::endl;
		}
		else if ((*it).compare("client_max_body_size") == 0)
		{
			it++;
			this->client_max_body_size = atoi((*it).c_str());
			//std::cout << "client_max_body_size = " << client_max_body_size << std::endl;
		}
		it++;
	}
}

void	Server_config::printConfig()
{
	std::cout << "listening port = " << listening_port << std::endl;
	std::cout << "server_name = " << server_name << std::endl;
	std::cout << "error_pages = " << error_code << std::endl;
	std::cout << "error_pages = " << error_path << std::endl;
	std::cout << "client_max_body_size = " << client_max_body_size << std::endl;
	
	std::map<std::string, Location*>::iterator it = this->locations.begin();
	while (it != this->locations.end())
	{
		std::cout <<"\t\tLOCATION_BLOCK \t " << it->first << std::endl;
		it->second->printConfig();
		it++;
	}
	
}