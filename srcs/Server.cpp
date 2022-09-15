#include "Server.hpp"

Server::Server(int domain, int service, int protocol, u_long interface, int port,int backlog)
: _domain(domain), _service(service), _protocol(protocol), _interface(interface),
_port(port), _backlog(backlog), _socket(-1)
{
	// std::cout << "Server default constructor called" << std::endl;
}

Server::Server(const Server & src)
{
	*this = src;
	// std::cout << "Server copy constructor called" << std::endl;
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
		
	}
	// std::cout << "Server assignment constructor called" << std::endl;
	return (*this);
}

Server::~Server()
{
	// std::cout << "Server default destructor called" << std::endl;
}


// ---------------------------------GETTERS

int		Server::getDomain() const
{
	return(_domain);
}

int		Server::getService() const
{
	return(_service);
}

int		Server::getProtocol() const
{
	return(_protocol);
}

u_long	Server::getInterface() const
{
	return(_interface);
}

int		Server::getPort() const
{
	return(_port);
}

int		Server::getBacklog() const
{
	return(_backlog);
}

int		Server::getSocket() const
{
	return(_socket);
}


// ----------------------------------SETTERS

void	Server::setDomain(int domain)
{
	_domain = domain;
}

void	Server::setService(int service)
{
	_service = service;
}

void	Server::setProtocol(int protocol)
{
	_protocol = protocol;
}

void	Server::setInterface(ulong interface)
{
	_interface = interface;
}

void	Server::setPort(int port)
{
	_port = port;
}

void	Server::setBacklog(int backlog)
{
	_backlog = backlog;
}

void	Server::printConfig() const
{
	std::cout << "----Server Configuration----" << std::endl;
	std::cout << "Domain IP: " << _domain << std::endl;
	std::cout << "Service : " << _service << std::endl;
	std::cout << "Protocol : " << _protocol << std::endl;
	std::cout << "Interface : " << _interface << std::endl;
	std::cout << "Socket Port : " << _port << std::endl;
	std::cout << "Maximum Queued connection allowed : " << _domain << std::endl;
}

void Server::launch()
{
	memset(&address, 0, sizeof(address));
	address.sin_family = _domain;
	address.sin_port = htons(_port);
	address.sin_addr.s_addr= htonl(_interface);

	_socket = socket(_domain, _service, _protocol);

	if(_socket == -1)
		throw ERROR_SOCKET_CREATION();
	if (bind(_socket, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		throw ERROR_SOCKET_BINDING();
	}
	if(listen(_socket, _backlog) < 0)
		throw ERROR_LISTENING_SERVER();

	std::cout << "===========WAITING FOR CONNECTION===========" << std::endl;
}

const char * Server::ERROR_SOCKET_CREATION::what() const throw()
{
	return("Server : Failed to create server socket...");
}

const char * Server::ERROR_SOCKET_BINDING::what() const throw()
{	
	return("Server : Failed to bind the socket ");
}

const char * Server::ERROR_LISTENING_SERVER::what() const throw()
{
	return("Server : Failed to start listening...");
}
