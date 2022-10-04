#ifndef SOCKET_HPP
# define SOCKET_HPP

class Socket;
#include "define.hpp"
#include "Epoll.hpp"
#include "HttpRequest.hpp"
#include "HttpManager.hpp"
#include <string>

class Socket
{
	public:
		Socket();
		Socket(const Socket &rhs);
		Socket(Epoll &epoll, t_socket sockfd);// TODO mettre infoserver
		~Socket();
		Socket &operator=(const Socket &rhs);
		void	setRequest(std::string req);
		const HttpRequest	&getRequest(void) const;
		void	readSocket();

	private:
		HttpManager *_manager;
		bool _init;

		HttpRequest	_request;
		t_socket	_fd;
		// TODO mettre infoserver en membre priver
};

#endif