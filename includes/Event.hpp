#ifndef EVENT_HPP
# define EVENT_HPP

#include "Epoll.hpp"

void	clientEvent(Epoll &epoll, std::map<t_socket, HttpManager> stockManager);
void	serverEvent(Epoll &epoll, std::map<t_socket, HttpManager> stockManager);

#endif