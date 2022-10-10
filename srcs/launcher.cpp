#include "Server.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "define.hpp"
#include "HttpManager.hpp"
#include <stdlib.h>

#include "Config.hpp"

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
