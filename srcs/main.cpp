#include "Epoll.hpp"
#include "Server.hpp"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "HttpRespond.hpp"

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

void	serverEvent(Epoll &epoll)
{
	Epoll::stockEventType::const_iterator		it;
	Epoll::stockServerType::const_iterator	itServ;
	t_socket newClient;

	t_sockaddr_in sin;
	int size = sizeof(sin);
	
	for (it = epoll.getAllEvents().begin(); it != epoll.getAllEvents().end(); it++)
	{
		/*je met le fd que je cherche et je recupere un iterateur sur un serveur*/
		itServ = epoll.getSockServ().find(it->data.fd);
		if (itServ == epoll.getSockServ().end())
			continue;
		sin = itServ->second.getAddress();
		// marche en allant prendre le socket dans le serveur
		newClient = accept(it->data.fd, (t_sockaddr *)&sin, (socklen_t *)&size);
		if (newClient == -1)
			throw std::exception();
		epoll.addClient(newClient);
	}
}

void	clientEvent(Epoll &epoll)
{
	Epoll::stockEventType::const_iterator	it;
	Epoll::stockClientType::const_iterator	itClient;
	char str[2048];

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
				std::cout << str;
				bzero(str,2048);
			}
			epoll.changeSocket(it->data.fd, EPOLLOUT);
			std::cout << str << "fin de la requete" << std::endl;
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
			
			HttpRespond resp(file_extract, "text/html");
			send(it->data.fd, resp.getHttpRespond().c_str(), resp.getHttpRespond().size(), MSG_NOSIGNAL);
			epoll.deleteClient(it->data.fd);
			//std::cout << "le fd :" << it->data.fd << "attends que l'on ecrive sur dessus" << std::endl; 
		}
		if (it->events & EPOLLRDHUP)
		{
			epoll.deleteClient(it->data.fd);
		}
	}
}

void routine(Epoll &epoll)
{
	while (1)
	{
		epoll.wait();
		// TODO resise vetor events
		serverEvent(epoll);
		clientEvent(epoll);
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	if (ac != 2)
		return 1;
	Server	server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, atoi(av[1]), 100);
	Epoll	epoll;
	try
	{
		server.launch();
		epoll.addServer(server.getSocket(), server);
		routine(epoll);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		if (errno)
			std::cerr << "Errno : " << strerror(errno) << std::endl;
		return(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
