#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <string.h>
# include <errno.h>

class Server
{
	public:
		Server(int domain, int service, int protocol, u_long interface, int port,int backlog);
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

		//Setters
		//TODO Degager les setter
		void	setDomain(int domain);
		void	setService(int service);
		void	setProtocol(int protocol);
		void	setInterface(ulong interface);
		void	setPort(int port);
		void	setBacklog(int backlog);

		void	printConfig() const;
		void	launch();

	private:
		int					_domain; // AF_INET, AF_INET6, AF_UNSPEC
		int					_service; // SOCK_STREAM, SOCK_DGRAM
		int					_protocol; // use 0 for "any"
		u_long				_interface; // needs to be set to INADDR_ANY
		int					_port;
		int					_backlog; // maximum number of queued clients
		struct sockaddr_in	_address;
		int					_socket;
};

#endif