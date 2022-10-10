#include "Socket.hpp"

Socket::Socket()
{
	_fd = -1;
	_init = false;
}

Socket::Socket(const Socket &rhs)
{
	*this = rhs;
}

// Socket::Socket(Epoll &epoll, t_socket sockfd)// TODO mettre infoserver
// {
// 	epoll.addClient(sockfd, *this);
// }

Socket::~Socket()
{
}

Socket &Socket::operator=(const Socket &rhs)
{
	_fd = rhs._fd;
	return *this;
}

const HttpRequest &Socket::getRequest( void ) const
{
	return (_request);
}

void	Socket::setRequest(std::string req)
{
	_request.setRequest(req);
	_request.parser(/*req*/);
}

void	Socket::readSocket()
{
//	recv(it->data.fd, str, 2048, MSG_DONTWAIT);

	recv(_fd, _str, 2048, MSG_DONTWAIT);
}