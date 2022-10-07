#include "Server.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "define.hpp"
#include "HttpManager.hpp"
#include <stdlib.h>

static void routine(Epoll &epoll)
{
	std::map<t_socket, HttpManager> stockManager;

	while (1)
	{
		//std::cout << "avant wait" << std::endl;
		epoll.wait();
		//std::cout << "apres wait" << std::endl;
		serverEvent(epoll, stockManager);
		clientEvent(epoll, stockManager);
	}
}

void launcher(char **av)
{
	Epoll	epoll;
	Server	server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, atoi(av[1]), 100);

	server.launch();
	epoll.addServer(server.getSocket(), server);
	routine(epoll);
}
