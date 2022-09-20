#include "Server.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include <stdlib.h>

static void routine(Epoll &epoll)
{
	while (1)
	{
		epoll.wait();
		serverEvent(epoll);
		clientEvent(epoll);
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
