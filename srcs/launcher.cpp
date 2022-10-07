#include "Server.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include <stdlib.h>

#include "Config.hpp"

static void routine(Epoll &epoll)
{
	while (1)
	{
		epoll.wait();
		serverEvent(epoll);
		clientEvent(epoll);
	}
}

void launcher(char *av)
{
	Epoll	epoll;
	Config configuration(av);

	configuration.print_all_conf();

	std::list<Server*>::iterator it = (configuration.getServersList()).begin();
	(*it)->launch();
	epoll.addServer((*it)->getSocket(), (*it));
	
	routine(epoll);
}
