/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServeurEvent.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:33 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 14:01:26 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Epoll.hpp"
#include "define.hpp"
#include "exceptWebserv.hpp"
#include "HttpManager.hpp"
#include <map>

void	serverEvent(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager)
{
	int										size;
	t_socket 								newClient;
	t_sockaddr_in							sin;
	Epoll::stockEventType::const_iterator	it;
	Epoll::stockServerType::const_iterator	itServ;

	size = sizeof(sin);
	for (it = epoll.getAllEvents().begin(); it != epoll.getAllEvents().end(); it++)
	{
		itServ = epoll.getSockServ().find(it->data.fd);
		if (itServ == epoll.getSockServ().end())
			continue;
		sin = itServ->second->getAddress();
		newClient = accept(it->data.fd, (t_sockaddr *)&sin, (socklen_t *)&size);
		if (newClient == -1)
			throw exceptWebserv("serverEvent : Accept new client failed");
		std::cout << "coucou" << std::endl;
		epoll.addClient(newClient, epoll.getSockServ().find(it->data.fd)->second); // on associe l evetn client a un serveur
		stockManager.insert(std::make_pair(newClient, HttpManager(newClient)));
		std::cout << "le fd : " << newClient << " est lie au server " << itServ->second->getPort() << std::endl;
	}
}
