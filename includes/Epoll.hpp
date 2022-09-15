#ifndef EPOLL_HPP
# define EPOLL_HPP

#include <set>
#include <vector>
#include <sys/epoll.h>


typedef int t_socket;
typedef struct epoll_event t_epoll_event;

class Epoll
{
	public:
		Epoll(const Epoll &rhs);
		Epoll();
		~Epoll();

		Epoll	&operator=(const Epoll &rhs);
		void	addClient(t_socket &sock);
		void	addServer(t_socket &sock);
		void	deleteClient(t_socket &sock);
		void	deleteServer(t_socket &sock);
		void	changeSocket(t_socket &sock,  t_epoll_event &event);
		void	wait();
	
		const std::set<t_socket> &getSockClient() const;
		const std::set<t_socket> &getSockServ() const;
		const std::vector<t_epoll_event> &getAllEvents() const; // ? peut-etre degage le const peut etre pratique

	private:
		t_socket							_instance;
		std::set<t_socket>					_sockServ;
		std::set<t_socket>					_sockClient;
		std::vector<t_epoll_event>			_AllEvents;
};

#endif