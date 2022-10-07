#ifndef EPOLL_HPP
# define EPOLL_HPP

#include <set>
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
		typedef	std::set<t_socket>			stockClientType;
		typedef	std::map<t_socket, Server>	stockServerType;
		
		Epoll(const Epoll &rhs);
		Epoll();
		~Epoll();

		Epoll	&operator=(const Epoll &rhs);
		void	addClient(t_socket const & sock);
		void	addServer(t_socket const & sock, Server const *server);
		void	deleteClient(t_socket const & sock);
		void	deleteServer(t_socket const & sock);
		void	changeSocket(t_socket const & sock, uint32_t mask_event);
		void	wait();

		const std::set<t_socket> &getSockClient() const;
		const std::map<t_socket,Server> &getSockServ() const;
		const std::vector<t_epoll_event> &getAllEvents() const; // ? peut-etre degage le const peut etre pratique

	private:
		t_socket					_instance;
		std::map<t_socket, Server>	_sockServ;
		std::set<t_socket>			_sockClient;
		std::vector<t_epoll_event>	_AllEvents;
};

#endif