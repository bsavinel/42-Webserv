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
		itClient = socketClient.find(it->data.fd);
		if (itClient == socketClient.end())
			continue;
		// TODO EPOLLHUPP pq trop compris mais ca a l'air chiant a gerer
        // TODO EPOLL_ONE SHOT sa a l'air chiant aussi

        if (it->events & EPOLLRDHUP || it->events & EPOLLERR)
        {
            epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
        }

        if (it->events & EPOLLIN)
            stockManager.find(it->data.fd)->second.receive();
        else 
            stockManager.find(it->data.fd)->second.setReadOk(false);

        if (stockManager.find(it->data.fd)->second.applyMethod(itClient->second /*Info server*/, *it/*flag event*/))
        {
            epoll.deleteClient(it->data.fd);
            stockManager.erase(it->data.fd);
        }

        if (it->events & EPOLLOUT)
            stockManager.find(it->data.fd)->second.sender();
        else 
            stockManager.find(it->data.fd)->second.setWriteOk(false);
		/*if (it->events & EPOLLIN)
		{
			bzero(str, 2048);
			while (recv(it->data.fd, str, 2048, MSG_DONTWAIT) > 0)
			{
				stockRequest += str;
				bzero(str, 2048);
			}
			epoll.changeSocket(it->data.fd, EPOLLOUT); // ? normallement ca sert a rien
			// std::cerr << "Debut de la requete" << std::endl << std::endl << stockRequest << "fin de la requete" << std::endl << std::endl;
			
			//HttpRequest request(stockRequest);
			//request.parser();
			Socket &tmp = socketClient.find(it->data.fd)->second;
			tmp.setRequest(stockRequest);
		}
		if (it->events & EPOLLOUT)
		{
			HttpResponse resp(it->data.fd);
//			std::cerr << "Debut de la requete" << std::endl << std::endl << socketClient.find(it->data.fd)->second.getRequest().getRequest() << "fin de la requete" << std::endl << std::endl;
			resp.buildRespond(socketClient.find(it->data.fd)->second.getRequest(), 213); //? send la reponse
			epoll.deleteClient(it->data.fd);
		}
		if (it->events & EPOLLRDHUP)
		{
			epoll.deleteClient(it->data.fd);
		}*/
	}
}
