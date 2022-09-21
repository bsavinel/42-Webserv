#include "Socket.hpp"

Socket::Socket()
{
	_fd = -1;
}

Socket::Socket(const Socket &rhs)
{
	*this = rhs;
}

Socket::Socket(Epoll &epoll, t_socket sockfd)// TODO mettre infoserver
{
	epoll.addClient(sockfd, *this);
}

Socket &Socket::operator=(const Socket &rhs)
{
	_fd = rhs._fd;
	return *this;
}

Socket::~Socket()
{
}
