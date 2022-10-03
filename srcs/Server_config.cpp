#include "Server_config.hpp"


Server_config::Server_config()
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
	// std::cout << "Server_config default destructor called" << std::endl;
}

void Server_config::getConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	while (it != splitted.end() && (*it).compare("}") != 0)
	{
		if((*it).compare("location") == 0)
		{
			while (it != splitted.end() && (*it).compare("}") != 0)
				it++;
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
}