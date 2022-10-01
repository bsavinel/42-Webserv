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
			std::cout << "Debut de la requete" << std::endl << stockRequest << "fin de la requete" << std::endl;
			
			/*HttpRequest request(stockRequest);
			request.parser();*/
		}
		if (it->events & EPOLLOUT)
		{
			// TODO mettre la lecture de fichier dans le httpRespond
			std::string file_extract;
			int fd;
			int i;
			fd = open("./ressources/index.html", O_RDONLY);
			bzero(str, 2048);
			i = read(fd, str, 2048);
			while (i > 0)
			{
				file_extract.append(str);
				bzero(str,2048);
				i = read(fd, str, 2048);
			}
			HttpResponse resp(it->data.fd);
			HttpRequest request(stockRequest);
			request.parser();
			resp.buildRespond(request, 213);
			//send(it->data.fd, resp.getHttpRespond().c_str(), resp.getHttpRespond().size(), MSG_NOSIGNAL);
			epoll.deleteClient(it->data.fd);
		}
		if (it->events & EPOLLRDHUP)
		{
			epoll.deleteClient(it->data.fd);
		}
	}
}
