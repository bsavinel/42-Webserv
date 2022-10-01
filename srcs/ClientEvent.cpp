#include "Epoll.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include <iostream>

// TODO changer le open en stream
#include <unistd.h>
#include <fcntl.h>

void	clientEvent(Epoll &epoll)
{
	Epoll::stockEventType::const_iterator	it;
	Epoll::stockClientType::const_iterator	itClient;
	char str[2048];
	std::string stockRequest;

	for (it = epoll.getAllEvents().begin(); it != epoll.getAllEvents().end(); it++)
	{
		itClient = epoll.getSockClient().find(it->data.fd);
		if (itClient == epoll.getSockClient().end())
			continue;
		if (it->events & EPOLLIN)
		{
			bzero(str,2048);
			while (recv(it->data.fd, str, 2048, MSG_DONTWAIT) > 0)
			{
				stockRequest += str;
				bzero(str,2048);
			}
			epoll.changeSocket(it->data.fd, EPOLLOUT);
			// std::cerr << "Debut de la requete" << std::endl << std::endl << stockRequest << "fin de la requete" << std::endl << std::endl;
			
			/*HttpRequest request(stockRequest);
			request.parser();*/
			Socket &tmp = epoll.getSockClient().find(it->data.fd)->second;
			tmp.setRequest(stockRequest);
		}
		if (it->events & EPOLLOUT)
		{
			HttpResponse resp(it->data.fd);
//			std::cerr << "Debut de la requete" << std::endl << std::endl << epoll.getSockClient().find(it->data.fd)->second.getRequest().getRequest() << "fin de la requete" << std::endl << std::endl;
			resp.buildRespond(epoll.getSockClient().find(it->data.fd)->second.getRequest(), 213); //? send la reponse
			epoll.deleteClient(it->data.fd);
		}
		if (it->events & EPOLLRDHUP)
		{
			epoll.deleteClient(it->data.fd);
		}
	}
}
