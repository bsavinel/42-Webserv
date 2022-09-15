#ifndef EPOLL_HPP
# define EPOLL_HPP

#include <set>
#include <vector>
#include <sys/epoll.h>
#include <string>

typedef int t_socket;
typedef struct epoll_event t_epoll_event;

class Epoll
{
	public:
		typedef	std::vector<t_epoll_event>	stockEventType;
		typedef	std::set<t_socket>			stockClientType;
		typedef	std::set<t_socket>			stockServerType;
		
		Epoll(const Epoll &rhs);
		Epoll();
		~Epoll();

		Epoll	&operator=(const Epoll &rhs);
		void	addClient(t_socket const & sock);
		void	addServer(t_socket const & sock);
		void	deleteClient(t_socket const & sock);
		void	deleteServer(t_socket const & sock);
		void	changeSocket(t_socket const & sock, uint32_t mask_event);
		void	wait();

		const std::set<t_socket> &getSockClient() const;
		const std::set<t_socket> &getSockServ() const;
		const std::vector<t_epoll_event> &getAllEvents() const; // ? peut-etre degage le const peut etre pratique

		class	EpollCreateFailed: public std::exception
		{
			const char *what() const throw();
		};

		class	EpollCtlFailed: public std::exception
		{
			const char *what() const throw();
		};

	private:
		t_socket							_instance;
		std::set<t_socket>					_sockServ;
		std::set<t_socket>					_sockClient;
		std::vector<t_epoll_event>			_AllEvents;
};

#endif