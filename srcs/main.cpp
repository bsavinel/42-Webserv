#include "Epoll.hpp"
#include "Server.hpp"
#include <cstdlib>

typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr t_sockaddr;

// TODO regarder si il faut changer l'etat apres avoir fait une action pour le prochain wait ou non on peut aussi garde en moire le fd non fini mais il y a un probleme pour le serveur
void	serverEvent(Epoll &epoll)
{
	Epoll::stockEventType::iterator		it;
	Epoll::stockServerType::iterator	itServ;
	
	t_sockaddr_in sin;
	int size = sizeof(sin);
	//! //BUG peut-etre qu'il va faloir mettre en public les truc de getteur
	for (it = epoll.getAllEvents().begin(); it != epoll.getAllEvents().end(); it++)
	{
		t_socket newClient;

		/*je met le fd que je cherche et je recupere un iterateur sur un serveur*/
		itServ = epoll.getSockServ().find(it->data.fd);
		if (epoll.getSockServ().find(it->data.fd) == epoll.getSockServ().end())
			continue;
		// TODO changer le stock des sesrver en map ayant pour cle le fd et pour valeur la classe serveur
		// TODO donc toute les fonction aui vont avec c'est a dire addServer et peut-etre deleteServer 
		// TODO avoir ajouter un getAdress a Serveur
		newClient = accept(itServ->getSocket(), (t_sockaddr *)&itServ->getAdress(), (socklen_t *)&size);
		epoll.addClient(newClient);
	}
}

void	clientEvent(Epoll &epoll)
{

}

void routine(Epoll &epoll, Server &server)
{
	while (1)
	{
		epoll.wait();
		serverEvent(epoll);
		clientEvent(epoll);
	}
}

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	Server	server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 100);
	Epoll	epoll;
	try
	{
		server.launch();
		routine(epoll, server);
		epoll.addServer(server.getSocket());
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
