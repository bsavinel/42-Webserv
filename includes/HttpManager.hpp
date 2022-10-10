#ifndef HTTPMANAGER_HPP
# define HTTPMANAGER_HPP

# include <string>
# include "define.hpp"
# include "define.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
#include <sys/types.h>
#include <sys/socket.h>
// TODO metre trois if pour repartir dans la methode adequate
// TODO changer dans epoll le map des clients, pour lie au manager le fd
// TODO mettre class Socket dans manager
// TODO changer ServeurEvent et ClientEvent
// TODO lie le serveur au fd dans socket

class HttpManager
{
	public:
		HttpManager(int sockClient);
		HttpManager(const HttpManager& rhs);

		~HttpManager();

		HttpManager		&operator=(const HttpManager& rhs);
		bool			applyMethod(int &InfoServer, t_epoll_event &event);
		void			setStock(std::string &stock);
		int				receive( void );
		void			parseHeader( void );

		/*Bsavinel*/

		void	setReadOk(bool read); // ? pas sur que se soit utile
		void	setWriteOk(bool write); // ? pas sur que se soit utile
		void	sender();

	protected:
		t_socket		_socketClient;

		void			getMethod();
		void			postMethod();
		void			deleteMethod();

		/*Bsavinel*/

		t_method		_method;// il peut sur un get
		bool			_init; 
		bool			_isEnd; // Sert a dire si la requete est fini
		bool			_Readok;
		bool			_Writeok;
		std::string		_respond;

		/*Rpottier*/

		std::string		_buffer;
		HttpRequest		_request;
		//HttpResponse	_response;
};

#endif
