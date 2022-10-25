#ifndef HTTPMANAGER_HPP
# define HTTPMANAGER_HPP

# include <string>
# include "define.hpp"
# include "define.hpp"
# include "HttpRequest.hpp"
# include <sys/types.h>
# include "Server.hpp"
# include <sys/socket.h>
# include <Cgi.hpp>

std::string HeaderRespond(off_t contentLenght, int statusCode, std::string type = std::string());

class HttpManager
{
	public:

		HttpManager(int sockClient);
		HttpManager(const HttpManager& rhs);
		
		~HttpManager();

		HttpManager			&operator=(const HttpManager& rhs);

		bool				applyMethod(const Server &server);
		int					receive( void );
		void				sender();
		void				initialize(const Server &server);

		/*Geter setter*/

		const bool			&getInit() const;
		const bool			&getWriteOk() const;
		const bool			&getReadOk() const;
		const bool    		&getIsEnd() const;
		const bool			&getModeChange() const;
		HttpRequest			&getRequest() const;
		const std::string	&getResponse() const;

		void				setModeChange(bool modeChange);

	private:

		int				_errorCode;
		t_socket		_socketClient;
		Cgi				_cgi;

		std::string		buildLocalPath();

		/*for outisde chose*/
		bool			_Writeok;
		bool			_Readok;
		bool			_modeChange;
		bool			_init;
		bool			_isEnd;

		void			canRead();
		void			canWrite();
		std::string		determinateType();

		/*initialisation*/


		/*Get methode*/

		std::string		LocalPathFile_get();
		void			getMethod();
		void			OpenFile_get(std::string &file_name);
		void			initialize_get();
		void			builRespondGet();

		int				_file;
		std::string		_name_file;
		bool			_headerBuild;

		/*Post methode*/

		void			postMethod();
		int				_tmp_upload_fd;

		/*delete methode*/

		void			deleteMethod();

		/*Reponse Requete*/

		std::string		ErrorRespond();
		std::string		_respond;
		HttpRequest		_request;
};

#endif
