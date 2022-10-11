#include "Epoll.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include <iostream>
#include <sys/epoll.h>

// TODO changer le open en stream
#include <unistd.h>
#include <map>
#include <fcntl.h>

void	clientEvent(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager)
{
	Epoll::stockEventType::iterator				it;
	std::map<t_socket, int> &					socketClient =	epoll.getSockClient();
	Epoll::stockClientType::iterator			itClient;
	//std::string stockRequest;
	//char str[2048];

	for (it = epoll.getAllEvents().begin(); it != epoll.getAllEvents().end(); it++)
	{
		//std::cout << "#######################tour de boucle " << std::endl;
		itClient = socketClient.find(it->data.fd);
		if (itClient == socketClient.end())
			continue;
		// TODO EPOLLHUPP pq trop compris mais ca a l'air chiant a gerer
        // TODO EPOLL_ONE_SHOT sa a l'air chiant aussi
        if (it->events & EPOLLRDHUP || it->events & EPOLLERR)
        {
            epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
        }
        if (it->events & EPOLLIN)
            stockManager.find(it->data.fd)->second.receive();
    	stockManager.find(it->data.fd)->second.applyMethod(itClient->second);
		// TODO mettre une fermeture a part
		//stockManager.find(it->data.fd)->second.sender();
		if (stockManager.find(it->data.fd)->second.getWriteOk())
		{
			std::cout << "ta mere" << std::endl;
			epoll.changeSocket(it->data.fd, EPOLLOUT);
			stockManager.find(it->data.fd)->second.sender();
		}
        if (it->events & EPOLLOUT)
		{
			//std::cout << "iciciccicicicicicicicicificicici" << std::endl;
            stockManager.find(it->data.fd)->second.sender();
		}
		
		if (stockManager.find(it->data.fd)->second.getIsEnd() == true)
		{
			epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
		}
	}
}
