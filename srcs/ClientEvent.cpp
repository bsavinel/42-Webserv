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
//		std::cout << "#######################tour de boucle " << std::endl;
		itClient = socketClient.find(it->data.fd);
		if (itClient == socketClient.end())
			continue;
		// TODO EPOLLHUPP pq trop compris mais ca a l'air chiant a gerer
        // TODO EPOLL_ONE SHOT sa a l'air chiant aussi
        if (it->events & EPOLLRDHUP || it->events & EPOLLERR)
        {
//			std::cout << "########### supression" << std::endl;
            epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
        }
        if (it->events & EPOLLIN)
        {
            std::cout << "Call Receive() " << std::endl;
            stockManager.find(it->data.fd)->second.receive();
            std::cout << "End Call Receive() " << std::endl;
        }
        else 
            stockManager.find(it->data.fd)->second.setReadOk(false);
        if (stockManager.find(it->data.fd)->second.applyMethod(itClient->second /*Info server*/, *it/*flag event*/))
        {
//			std::cout << "########### supression" << std::endl;
            epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
        }

        if (it->events & EPOLLOUT)
            stockManager.find(it->data.fd)->second.sender();
        else 
            stockManager.find(it->data.fd)->second.setWriteOk(false);
	}
	//std::cout << "Fin de event client" << std::endl;
	
}
