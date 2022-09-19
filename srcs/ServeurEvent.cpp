#include "Epoll.hpp"

// Mettre un fichier pour les typedef et les define

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