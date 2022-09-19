#ifndef EVENT_HPP
# define EVENT_HPP

#include "Epoll.hpp"

void	clientEvent(Epoll &epoll);
void	serverEvent(Epoll &epoll);

#endif