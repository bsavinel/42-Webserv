#include "Epoll.hpp"
#include <unistd.h>

Epoll::Epoll()
{
	_instance = epoll_create(10);
}

Epoll::Epoll(const Epoll &rhs)
{
	*this = rhs;
}

/* Creer instance epoll et ajoute le serveur si donne en parametre"*/
Epoll::~Epoll()
{
	std::set<t_socket>::iterator ptr;

	for (ptr = _sockClient.begin(); ptr != _sockClient.end(); ptr++)
		close(*ptr);
	for (ptr = _sockServ.begin(); ptr != _sockServ.end(); ptr++)
		close(*ptr);
	close(_instance);
}

/*Supprime tout les socket et ferme l'instance epoll*/
Epoll &Epoll::operator=(const Epoll &rhs)
{
	this->_instance = rhs._instance;
	this->_sockServ = rhs._sockServ;
	this->_sockClient = rhs._sockClient;
	this->_AllEvents = rhs._AllEvents;
	return *this;
}

void	Epoll::addClient(t_socket &sock)
{
	t_epoll_event epollEvent;

	_sockClient.insert(sock);
	epollEvent.data.fd = sock;
	epollEvent.events = EPOLLIN;
	epoll_ctl(_instance, EPOLL_CTL_ADD, sock, &epollEvent);
}

void	Epoll::addServer(t_socket &sock)
{
	t_epoll_event epollEvent;

	_sockServ.insert(sock);
	epollEvent.data.fd = sock;
	epollEvent.events = EPOLLIN;
	epoll_ctl(_instance, EPOLL_CTL_ADD, sock, &epollEvent);
}

void	Epoll::deleteClient(t_socket &sock)
{
	t_epoll_event epollEvent;

	_sockClient.erase(sock);
	epoll_ctl(_instance, EPOLL_CTL_DEL, sock, &epollEvent);
	close(sock);
}

void	Epoll::deleteServer(t_socket &sock)
{
	t_epoll_event epollEvent;

	_sockServ.erase(sock);
	epoll_ctl(_instance, EPOLL_CTL_DEL, sock, &epollEvent);
	close(sock);
}


void	Epoll::changeSocket(t_socket &sock, t_epoll_event &event)
{
	epoll_ctl(_instance, EPOLL_CTL_MOD, sock, &event);
}

void	Epoll::wait()
{
	_AllEvents.resize(_sockClient.size() + _sockServ.size());
	epoll_wait(_instance, _AllEvents.data(), _sockClient.size() + _sockServ.size(), -1);
}
/*Mettre un vector dans la fonction puis itere avec plage iterateur pour inserr dans map*/ 
const std::set<t_socket> &Epoll::getSockClient() const
{
	return _sockClient;
}

const std::set<t_socket> &Epoll::getSockServ() const
{
	return _sockServ;
}

const std::vector<t_epoll_event> &Epoll::getAllEvents() const
{
	return _AllEvents;
}
