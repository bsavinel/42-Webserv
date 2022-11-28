/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServeurEvent.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:33 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:43:34 by rpottier         ###   ########.fr       */
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
		/*je met le fd que je cherche et je recupere un iterateur sur un serveur*/
		itServ = epoll.getSockServ().find(it->data.fd);
		if (itServ == epoll.getSockServ().end())
			continue;
		sin = itServ->second->getAddress();
		newClient = accept(it->data.fd, (t_sockaddr *)&sin, (socklen_t *)&size);
		if (newClient == -1)
			throw exceptWebserv("serverEvent : Accept new client failed");
		//Socket(epoll, newClient); // client est add a epoll a l'interieur
		epoll.addClient(newClient, epoll.getSockServ().find(it->data.fd)->second); // on associe l evetn client a un serveur
		stockManager.insert(std::make_pair(newClient, HttpManager(newClient)));
		std::cout << "le fd : " << newClient << " est lie au server " << itServ->second->getPort() << std::endl;
	}
}
