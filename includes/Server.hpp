#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <sys/socket.h>
# include <string.h>
# include <errno.h>
# include <iostream>
# include "exceptWebserv.hpp"
# include "Server_config.hpp"

class Server
{
	private:
		int					_domain; // AF_INET, AF_INET6, AF_UNSPEC
		int					_service; // SOCK_STREAM, SOCK_DGRAM
		int					_protocol; // use 0 for "any"
		u_long				_interface; // needs to be set to INADDR_ANY
		int					_port;
		int					_backlog; // maximum number of queued clients
		struct sockaddr_in	_address;
		int					_socket;

	public:
		//Server(int domain, int service, int protocol, u_long interface, int port,int backlog);
		Server(Server_config* server_config);
		Server(const Server & src);
		Server & operator=(const Server & rhs);
		~Server();

		//Getters
		int		getDomain() const;
		int		getService() const;
		int		getProtocol() const;
		u_long	getInterface() const;
		int		getPort() const;
		int		getBacklog() const;
		int		getSocket() const;
		struct sockaddr_in	getAddress() const;

		void	printConfig() const;
		void	launch();

		
};

#endif