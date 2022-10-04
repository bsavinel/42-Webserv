#ifndef HTTPMANAGER_HPP
# define HTTPMANAGER_HPP

# include <string>
# include "define.hpp"
# include "define.hpp"
# include "HttpRequest.hpp"

// TODO metre trios if por repartir dans la mthode adequate
// TODO changer dans epoll pour lie au manager
// TODO mettre class Socket dans manager
// TODO changer Serveurevent et client event
// TODO lie le serveur au fd dans socket

class HttpManager
{
	public:
		HttpManager();
		~HttpManager();
		void	applyMethod();
		void	getMethod();
		void	postMethod();
		void	deleteMethod();
		void	setStock(std::string &stock);

	protected:
		std::string	_stock;
		t_method	_method;
		HttpRequest	_request;
};

#endif
