#ifndef EPOLL_HPP
# define EPOLL_HPP

class Epoll;

#include <vector>
#include <sys/epoll.h>
#include <string>
#include <map>
#include "Server.hpp"
#include "define.hpp"
#include <exception>

class Epoll
{
	public:
		typedef	std::vector<t_epoll_event>	stockEventType;
		typedef	std::map<t_socket, Server>	stockClientType;
		typedef	std::map<t_socket, Server>	stockServerType;
		
		Epoll(const Epoll &rhs);
		Epoll();
		~Epoll();

		Epoll	&operator=(const Epoll &rhs);
		void	addClient(t_socket const & sock, Server const * infoServ);
		void	addServer(t_socket const & sock, Server const * server);
		void	deleteClient(t_socket const & sock);
		void	deleteServer(t_socket const & sock);
		void	changeSocket(t_socket const & sock, uint32_t mask_event);
		void	wait();

		std::map<t_socket, Server>				&getSockClient();
		const std::map<t_socket, Server>	&getSockServ() const;
		std::vector<t_epoll_event>	&getAllEvents(); // ? peut-etre degage le const peut etre pratique

	private:
		t_instEpoll					_instance;
		std::map<t_socket, Server>	_sockServ;
		std::map<t_socket, Server>	_sockClient;
		std::vector<t_epoll_event>	_AllEvents;
};

#endif