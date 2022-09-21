#include "Socket.hpp"

Socket::Socket()
{
	fd = -1;
	epfd = -1;
}

Socket::Socket(const Socket &rhs)
{
	fd = rhs.fd;
    epfd = rhs.epfd;
}

Socket::Socket(Epoll &epoll, t_socket sockfd): _epoll(epoll)// TODO mettre infoserver
{
}

Socket::~Socket()
{
}

