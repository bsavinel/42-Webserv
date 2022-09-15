#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <string.h>
# include <errno.h>

class Server
{
	private :
		int					_domain; // AF_INET, AF_INET6, AF_UNSPEC
		int					_service; // SOCK_STREAM, SOCK_DGRAM
		int					_protocol; // use 0 for "any"
		u_long				_interface; // needs to be set to INADDR_ANY
		int					_port;
		int					_backlog; // maximum number of queued clients
		struct sockaddr_in	address;
		int					_socket;

	public :
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

		//Setters
		void	setDomain(int domain);
		void	setService(int service);
		void	setProtocol(int protocol);
		void	setInterface(ulong interface);
		void	setPort(int port);
		void	setBacklog(int backlog);


		void	printConfig() const;
		void	launch();

		class ERROR_SOCKET_CREATION : public std::exception
		{
			const char * what() const throw();
		};

		class ERROR_SOCKET_BINDING : public std::exception
		{
			const char * what() const throw();
		};

		class ERROR_LISTENING_SERVER : public std::exception
		{
			const char * what() const throw();
		};
};

#endif