#include "Epoll.hpp"
#include "HttpRequest.hpp"
#include <iostream>
#include <sys/epoll.h>

// TODO changer le open en stream
#include <unistd.h>
#include <map>
#include <fcntl.h>

static void print_status(t_epoll_event event)
{
	std::cout << "le fd : " << event.data.fd << " a les evenement: ";
	if (event.events & EPOLLIN)
		std::cout << "EPOLLIN";
	if (event.events & EPOLLOUT)
		std::cout << "EPOLLOUT";
	if (event.events & EPOLLONESHOT)
		std::cout << "EPOLLONESHOT";
	if (event.events & EPOLLEXCLUSIVE)
		std::cout << "EPOLLEXCLUSIVE";
	std::cout << std::endl;
}

void	clientEvent(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager)
{
	Epoll::stockEventType::iterator				it;
	std::map<t_socket, Server> &				socketClient =	epoll.getSockClient();
	Epoll::stockClientType::iterator			itClient;
	//std::string stockRequest;
	//char str[2048];

	for (it = epoll.getAllEvents().begin(); it != epoll.getAllEvents().end(); it++)
	{
		itClient = socketClient.find(it->data.fd);
		if (itClient == socketClient.end())
			continue;
		HttpManager &manager = stockManager.find(it->data.fd)->second;

		print_status(*it);
		if (manager.getIsEnd() || it->events & EPOLLRDHUP || it->events & EPOLLERR)
		{
			std::cout << "Fd : " << it->data.fd << " a ete suprimer" << std::endl;
			epoll.deleteClient(it->data.fd);
			stockManager.erase(it->data.fd);
			continue ;
		}


        if (it->events & EPOLLIN)
            manager.receive();
	
		if (manager.getInit() == false)
			manager.initialize(socketClient.find(it->data.fd)->second);
    	manager.applyMethod();

		if (manager.getModeChange() && manager.getWriteOk())
		{
			epoll.changeSocket(it->data.fd, EPOLLOUT);
			manager.setModeChange(false);
		}

		if (manager.getModeChange() && manager.getReadOk())
		{
			epoll.changeSocket(it->data.fd, EPOLLIN);
			manager.setModeChange(false);
		}

		if (it->events & EPOLLOUT)
			manager.sender();
	}
}
