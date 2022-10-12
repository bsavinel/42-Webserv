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

		print_status(*it);
		// TODO EPOLLHUPP pq trop compris mais ca a l'air chiant a gerer
        // TODO EPOLL_ONE_SHOT sa a l'air chiant aussi



        if (it->events & EPOLLRDHUP || it->events & EPOLLERR)
        {
            epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
        }
        if (it->events & EPOLLIN)
            stockManager.find(it->data.fd)->second.receive();
    	stockManager.find(it->data.fd)->second.applyMethod(socketClient.find(it->data.fd)->second);
		if (stockManager.find(it->data.fd)->second.getWriteOk())
		{
			epoll.changeSocket(it->data.fd, EPOLLOUT);
			//stockManager.find(it->data.fd)->second.sender();
		}
      /*  if (it->events & EPOLLOUT)
            stockManager.find(it->data.fd)->second.sender();*/
		stockManager.find(it->data.fd)->second.sender();
		if (stockManager.find(it->data.fd)->second.getIsEnd() == true)
		{
			std::cout << "#############supression" << std::endl;
			epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
		}
	}
}
