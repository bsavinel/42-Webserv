#ifndef SOCKET_HPP
# define SOCKET_HPP

#include "define.hpp"
#include "Epoll.hpp"
class Socket
{
	public:
		Socket();
		Socket(const Socket &rhs);
		Socket(Epoll &epoll, t_socket sockfd);// TODO mettre infoserver
		~Socket();
		
	private:	
		int _fd;
		// TODO mettre infoserver en membre priver
};

#endif