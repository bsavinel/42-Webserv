/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:33 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:34 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		epoll.wait();
		serverEvent(epoll, stockManager);
		clientEvent(epoll, stockManager);
	}
}

bool	check_server_port(Config &configuration)
{
	std::list<Server*>::iterator itServer = (configuration.getServersList()).begin();
	std::list<Server*>::iterator itNextServer;
	std::list<Server*>::iterator ite =  (configuration.getServersList()).end();
	while (itServer != ite)
	{
		int port = (*itServer)->getPort();
		itNextServer = itServer;
		itNextServer++;
		while (itNextServer != ite)
		{
			if (port == (*itNextServer)->getPort())
				return(0);
			itNextServer++;
		}
		itServer++;
	}
	return(1);
}

void launcher(char *av)
{
	Epoll	epoll;
	Config	configuration;

	configuration.init(av);

	//configuration.print_all_conf();

	if(!check_server_port(configuration))
		throw exceptWebserv("Error : Two servers set on the same port");

	std::list<Server*>::iterator itServer = (configuration.getServersList()).begin();
	while(itServer != (configuration.getServersList()).end())
	{
		(*itServer)->launch();
		epoll.addServer((*itServer)->getSocket(), (*itServer));
		itServer++;
	}
	routine(epoll);
}
