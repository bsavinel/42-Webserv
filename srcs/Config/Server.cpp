/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:28 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:43:29 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
: _domain(AF_INET), _service(SOCK_STREAM), _protocol(0), _interface(INADDR_ANY),
_backlog(200), _socket(-1), client_max_body_size(0)
{

}

Server::Server(const Server & src)
{
	*this = src;
}

Server & Server::operator=(const Server & rhs)
{
	if(this != &rhs)
	{
		_domain = rhs._domain;
		_service = rhs._service;
		_protocol = rhs._protocol;
		_interface = rhs._interface;
		_port = rhs._port;
		_backlog = rhs._backlog;
		_socket = rhs._socket;
		server_name = rhs.server_name;
		error_map = rhs.error_map;
		client_max_body_size = rhs.client_max_body_size;
		locations = rhs.locations;
	}
	return (*this);
}

Server::~Server()
{
	for( std::map<std::string, Location*>::iterator it = locations.begin(); it != locations.end(); it++)
		delete	it->second;
}


void Server::launch()
{
	const int enable = 1;
	_address.sin_family = _domain;
	_address.sin_port = htons(_port);
	_address.sin_addr.s_addr= htonl(_interface);
	_socket = socket(_domain, _service, _protocol);

	if(_socket == -1)
		throw exceptWebserv("Server : Failed to create server socket");
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
   		throw exceptWebserv("Server : Failed to reuse server socket");
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) == -1)
	{
		throw exceptWebserv("Server : Failed to bind the socket");
	}
	if(listen(_socket, _backlog) < 0)
		throw exceptWebserv("Server : Failed to start listening");
	std::cout << "Port :" << _port << std::endl;
}

void Server::setConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	bool listenIsSet = false;
	it++;
	while (it != splitted.end() && (*it).compare("}") != 0)
	{
		if((*it).compare("location") == 0)
			add_location_block(it, splitted);
		else if ((*it).compare("listen") == 0 && (*(it + 1)) != ";" && (*(it + 2)) == ";")
		{
			this->_port = atoi(((*++it).c_str()));
			listenIsSet = true;
		}
		else if ((*it).compare("server_name") == 0 && (*(it + 1)) != ";" && (*(it + 2)) == ";")
			this->server_name = *++it;
		else if ((*it).compare("error_pages") == 0 && (*(it + 1)) != ";" && (*(it + 3)) == ";")
			set_error_pages(it);
		else if ((*it).compare("client_max_body_size") == 0 && (*(it + 1)) != ";" && (*(it + 2)) == ";" )
			set_client_max_body_size(it);
		else if (*it != ";")
		{
			std::cerr << "HERE" << std::endl;
			throw exceptWebserv ("Error Server : option not compatible or missing \';\'");
		}
		it++;
	}
	if (listenIsSet == false || _port < 0)
		throw exceptWebserv ("Error Server : listening port not set or bad value attributed");
}




// ------------------------------------------------------------------------------------------
// |										GETTERS											|
// ------------------------------------------------------------------------------------------

const int&		Server::getDomain() const
{
	return(_domain);
}

const int&		Server::getService() const
{
	return(_service);
}

const int&		Server::getProtocol() const
{
	return(_protocol);
}

u_long	Server::getInterface() const
{
	return(_interface);
}

const int&		Server::getPort() const
{
	return(_port);
}

const int&		Server::getBacklog() const
{
	return(_backlog);
}

const int&		Server::getSocket() const
{
	return(_socket);
}

struct sockaddr_in	Server::getAddress() const
{
	return (_address);
}

const std::string&	Server::getServerName() const
{
	return(this->server_name);
}

const int&	Server::getClientMaxBodySize() const
{
	return(this->client_max_body_size);
}

std::map<std::string, Location*> Server::getLocationsMap() const
{
	return(this->locations);
}

const std::map<int, std::string> &Server::getErrorMap() const
{
	return(this->error_map);
}





// ------------------------------------------------------------------------------------------
// |										PRINTS											|
// ------------------------------------------------------------------------------------------

void	Server::printConfig()
{
	std::cout << "----Server Configuration----" << std::endl;
	std::cout << "port = " << _port << std::endl;
	std::cout << "server_name = " << server_name << std::endl;
	std::cout << "-----Map Error Content------" << std::endl;
	for(std::map<int, std::string>::iterator iterr = error_map.begin(); iterr != error_map.end(); iterr++)
	{
		std::cout << iterr->first << iterr->second << std::endl;
	}
	std::cout << "client_max_body_size = " << client_max_body_size << std::endl;
	std::cout << "Domain IP: " << _domain << std::endl;
	std::cout << "Service : " << _service << std::endl;
	std::cout << "Protocol : " << _protocol << std::endl;
	std::cout << "Interface : " << _interface << std::endl;
	std::cout << "Maximum Queued connection allowed : " << _domain << std::endl;

	std::map<std::string, Location*>::iterator itloc = locations.begin();
	while (itloc != this->locations.end())
	{
		std::cout <<"\tLOCATION_BLOCK\t " << itloc->first << std::endl;
		itloc->second->printConfig();
		std::cout << std::endl;
		itloc++;
	}
	
}

// ------------------------------------------------------------------------------------------
// |									PRIVATE METHODS										|
// ------------------------------------------------------------------------------------------

bool	Server::is_path_stored_yet(std::string path)
{
	for(std::map<std::string, Location*>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		if(path == it->first)
			return(true);
	}
	return(false);
}

void	Server::add_location_block(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	it++;
	std::string path_loc;
	if((*it) != ";")
		path_loc = *it;
	if(!is_path_stored_yet(path_loc) && *(it + 1) == "{")
	{
		Location *new_loc = new Location();
		try
		{
			new_loc->setConfig(it, splitted, path_loc);
		}
		catch(const exceptWebserv& e)
		{
			std::cerr << e.what() << std::endl;
			delete new_loc;
			new_loc = NULL;
			throw exceptWebserv("Error Location : cannot initialize location block");
		}
		this->locations.insert(std::make_pair(path_loc, new_loc));
	}
}

void	Server::set_error_pages(std::vector<std::string>::iterator & it)
{
	int error_num = atoi((*++it).c_str());
	if(!check_existing_error_code(error_num))
		throw exceptWebserv("Error Config : error_code does not exist");
	std::string path_to_file = *++it;
	if(!is_file_path(path_to_file))
		throw exceptWebserv("Error Config : error_pages value should be a path to a file");
	this->error_map.insert(std::make_pair(error_num, path_to_file));
}

void	Server::set_client_max_body_size(std::vector<std::string>::iterator & it)
{
	const char *arg = (*++it).c_str();
	for(size_t i = 0; i < strlen(arg); i++)
	{
		if(!isdigit(arg[i]))
			throw exceptWebserv("Error Config : client_max_body_size need to be a number between 0 and MAXINT");
	}
	long conv = atol(arg);
	if (conv > INT_MAX)
		throw exceptWebserv("Error Config : client_max_body_size need to be a number between 0 and MAXINT");
	this->client_max_body_size = atoi(arg);
}

bool Server::checkRacineLocationExist()
{
    std::map<std::string, Location*>::iterator itLocation = locations.begin();
    std::map<std::string, Location*>::iterator endItLocation = locations.end();

    for (;itLocation !=endItLocation; itLocation ++)
    {
        if (itLocation->first == "/")
            return (true);
    }
    return (false);
}
// ------------------------------------------------------------------------------------------
// |										EXCEPTION										|
// ------------------------------------------------------------------------------------------

Server::exceptionServer::exceptionServer(const std::string content) throw()
{
	_content = content;
}

Server::exceptionServer::~exceptionServer() throw()
{

}

const char *Server::exceptionServer::what() const throw()
{
	return _content.c_str();
}

