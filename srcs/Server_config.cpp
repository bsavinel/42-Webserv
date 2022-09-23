#include "Server_config.hpp"


Server_config::Server_config(char *config_file)
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