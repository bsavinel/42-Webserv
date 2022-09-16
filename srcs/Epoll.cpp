#include "Epoll.hpp"
#include <unistd.h>
#include <exception>

Epoll::Epoll()
{
	_instance = epoll_create(10);
	if (_instance == -1)
		throw EpollCreateFailed();
}

Epoll::Epoll(const Epoll &rhs)
{
	*this = rhs;
}

Epoll::~Epoll()
{
	Epoll::stockClientType::const_iterator	itClient;
	Epoll::stockServerType::const_iterator	itServ;


	for (itClient = _sockClient.begin(); itClient != _sockClient.end(); itClient++)
		deleteClient(*itClient);
	for (itServ = _sockServ.begin(); itServ != _sockServ.end(); itServ++)
		deleteServer(itServ->first);
	close(_instance);
}

Epoll &Epoll::operator=(const Epoll &rhs)
{
	this->_instance = rhs._instance;
	this->_sockServ = rhs._sockServ;
	this->_sockClient = rhs._sockClient;
	this->_AllEvents = rhs._AllEvents;
	return *this;
}

void	Epoll::addClient(t_socket const & sock)
{
	t_epoll_event epollEvent;

	_sockClient.insert(sock);
	epollEvent.data.fd = sock;
	epollEvent.events = EPOLLIN;
	if (epoll_ctl(_instance, EPOLL_CTL_ADD, sock, &epollEvent))
		throw EpollCtlFailed();
}

void	Epoll::addServer(t_socket const & sock, Server const & server)
{
	t_epoll_event epollEvent;

	_sockServ.insert(std::make_pair(sock, server));
	epollEvent.data.fd = sock;
	epollEvent.events = EPOLLIN;
	if (epoll_ctl(_instance, EPOLL_CTL_ADD, sock, &epollEvent))
		throw EpollCtlFailed();
}

void	Epoll::deleteClient(t_socket const & sock)
{
	t_epoll_event epollEvent;

	_sockClient.erase(sock);
	epoll_ctl(_instance, EPOLL_CTL_DEL, sock, &epollEvent);
	close(sock);
}

void	Epoll::deleteServer(t_socket const & sock)
{
	t_epoll_event epollEvent;

	_sockServ.erase(sock);
	epoll_ctl(_instance, EPOLL_CTL_DEL, sock, &epollEvent);
	close(sock);
}


void	Epoll::changeSocket(t_socket const & sock, uint32_t mask_event)
{
//	epoll_ctl(_instance, EPOLL_CTL_MOD, sock, &event);
	t_epoll_event epollEvent;

	epollEvent.data.fd = sock;
	epollEvent.events = mask_event;
	epoll_ctl(_instance, EPOLL_CTL_MOD, sock, &epollEvent);
}

void	Epoll::wait()
{
	_AllEvents.resize(_sockClient.size() + _sockServ.size());
	epoll_wait(_instance, _AllEvents.data(), _sockClient.size() + _sockServ.size(), -1);
}

const std::set<t_socket> &Epoll::getSockClient() const
{
	return _sockClient;
}

const std::map<t_socket,Server> &Epoll::getSockServ() const
{
	return _sockServ;
}

const std::vector<t_epoll_event> &Epoll::getAllEvents() const
{
	return _AllEvents;
}

const char * Epoll::EpollCreateFailed::what() const throw()
{	
	return("Epoll : Failed to init instance");
}


const char * Epoll::EpollCtlFailed::what() const throw()
{	
	return("Epoll : Failed to add new socket in instance");
}