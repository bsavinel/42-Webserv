#ifndef HTTPMANAGER_HPP
# define HTTPMANAGER_HPP

# include <string>
# include "define.hpp"
# include "define.hpp"
# include "HttpRequest.hpp"
#include <sys/types.h>
#include "Server.hpp"
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

		HttpManager			&operator=(const HttpManager& rhs);

		bool				applyMethod();
		int					receive( void );
		void				sender();
		void				initialize(const Server &server);

		/*Geter setter*/

		const bool			&getInit() const;
		const bool			&getWriteOk() const;
		const bool			&getReadOk() const;
		const bool    		&getIsEnd() const;
		const bool			&getModeChange() const;
		const std::string	&getResponse() const;

		void				setModeChange(bool modeChange);

	private:

		Server			_server;
		t_socket		_socketClient;

		/*for outisde chose*/
		bool			_Writeok;
		bool			_Readok;
		bool			_modeChange;
		bool			_init;
		bool			_isEnd;

		void			canRead();
		void			canWrite();

		/*initialisation*/

		void			parseHeader( void );

		/*Get methode*/

		void			getMethod();
		void			initialize_get();
		void 			buildHeaderGet(off_t size);
		void			builRespondGet();
		bool			_endGet;
		int				_file;
		bool			_headerBuild;

		/*Post methode*/

		void			postMethod();

		/*delete methode*/

		void			deleteMethod();

		/*Reponse Requete*/

		std::string		_respond;
		HttpRequest		_request;
};

#endif
