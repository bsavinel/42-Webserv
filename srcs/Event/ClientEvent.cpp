//#include "Epoll.hpp"
#include "define.hpp"
#include "HttpManager.hpp"
#include "Epoll.hpp"
#include <iostream>
#include <sys/epoll.h>

// TODO changer le open en stream
#include <unistd.h>
#include <map>
#include <fcntl.h>

// static void print_status(t_epoll_event event)
// {
// //	std::cout << "le fd : " << event.data.fd << " a les evenement: ";
// 	if (event.events & EPOLLIN)
// 		std::cout << "EPOLLIN";
// 	if (event.events & EPOLLOUT)
// 		std::cout << "EPOLLOUT";
// 	if (event.events & EPOLLONESHOT)
// 		std::cout << "EPOLLONESHOT";
// 	if (event.events & EPOLLEXCLUSIVE)
// 		std::cout << "EPOLLEXCLUSIVE";
// 	if (event.events & EPOLLERR)
// 		std::cout << "EPOLLERR";
// 	if (event.events & EPOLLHUP)
// 		std::cout << "EPOLLHUP";
// 	std::cout << std::endl;
// }

/*void endOfRequest(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager, HttpManager &manager, int fdSock, Server &severOfSocket)
{
	if (manager.getClassRequest().getConnection().second == true &&
		manager.getClassRequest().getConnection().first == "keep-alive")
	{
		manager.setIsEnd(false);
		if (manager.getClassRequest().headerComplete()) //TODO header complete doit voir si il y a un /n/r/n/r dans le reste  de la requete 
		{
			manager.initialize(severOfSocket);
			// TODO avoir un getter de la content lenght pour savoir si on fini de lire la requete
			if (manager.getClassRequest().getContentLenght().second && 
				manager.getClassRequest().getContentLenght().fisrt < manager.getClassRequest().getRequest().size())
				epoll.changeSocket(fdSock, EPOLLIN);
		}
		else
		{
			manager.setInit(false);
			epoll.changeSocket(fdSock, EPOLLIN);
		}
	}
	else
	{
		std::cout << "Fd : " << fdSock << " a ete suprimer par une fin de traitement" << std::endl;
		epoll.deleteClient(fdSock);
		stockManager.erase(fdSock);
	}
}*/

void	clientEvent(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager)
{
	Epoll::stockEventType::iterator				it_event;
	std::map<t_socket, const Server *> &		socketClient =	epoll.getSockClient();
	Epoll::stockClientType::iterator			itClient;

	for (it_event = epoll.getAllEvents().begin(); it_event != epoll.getAllEvents().end(); it_event++)
	{
		itClient = socketClient.find(it_event->data.fd);
		if (itClient == socketClient.end())
			continue;
		HttpManager &manager = stockManager.find(it_event->data.fd)->second;

//		print_status(*it_event);
		
		if (it_event->events & EPOLLRDHUP || it_event->events & EPOLLERR)
		{
			std::cout << "Fd : " << it_event->data.fd << " a ete suprimer par evenement" << std::endl;
			epoll.deleteClient(it_event->data.fd);
			stockManager.erase(it_event->data.fd);
			continue ;
		}

		if (it_event->events & EPOLLIN)
			manager.receiver();
	
		if (manager.getInit() == false)
			manager.initialize(*socketClient.find(it_event->data.fd)->second);
		manager.applyMethod(*socketClient.find(it_event->data.fd)->second);

		if (manager.getModeChange() && manager.getWriteOk())
		{
			epoll.changeSocket(it_event->data.fd, EPOLLOUT);
			manager.setModeChange(false);
		}

		if (manager.getModeChange() && manager.getReadOk())
		{
			epoll.changeSocket(it_event->data.fd, EPOLLIN);
			manager.setModeChange(false);
		}
		

		if (it_event->events & EPOLLOUT)
			manager.sender();

		if (manager.getIsEnd())
		{
			//endOfRequest(epoll, stockManager, manager, it_event->data.fd, socketClient.find(it_event->data.fd)->second);
			std::cout << "Fd : " << it_event->data.fd << " a ete suprimer par une fin de traitement" << std::endl;
			epoll.deleteClient(it_event->data.fd);
			stockManager.erase(it_event->data.fd);
			continue ;
		}
	}
}
