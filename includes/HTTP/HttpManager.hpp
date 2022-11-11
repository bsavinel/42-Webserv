#ifndef HTTPMANAGER_HPP
# define HTTPMANAGER_HPP

class HttpManager;

# include <string>
# include "define.hpp"
# include "define.hpp"
# include "HttpRequest.hpp"
# include "Cgi.hpp"
# include <sys/types.h>
# include "Server.hpp"
# include <sys/socket.h>



std::string HeaderRespond(off_t contentLenght, int statusCode, std::string type = std::string() /*std::string &cookie = std::string()*/);

class HttpManager
{
	public:

		HttpManager(int sockClient);
		HttpManager(const HttpManager& rhs);
		
		~HttpManager();

		HttpManager			&operator=(const HttpManager& rhs);

		bool				applyMethod(const Server &server);
		bool				check_if_method_authorized();
		void				launch_cgi(HttpRequest &_request, const Server &server);
		int					receive( void );
		void				sender();
		void				initialize(const Server &server);

		/*Geter setter*/

		const bool			&getInit() const;
		const bool			&getWriteOk() const;
		const bool			&getReadOk() const;
		const bool    		&getIsEnd() const;
		const bool			&getModeChange() const;
		const HttpRequest	&getRequest() const;
		const std::string	&getResponse() const;
		const HttpRequest	&getClassRequest() const;

		void				setModeChange(bool modeChange);
		void				setErrorCode(int statusCode);
		void				setIsEnd(bool IsEnd);
		void				setInit(bool init);

	private:

		int				_errorCode;
		t_socket		_socketClient;

		/* CGI */
		Cgi				_cgi;
		bool			_firstPassage;
		bool			_proccess_fini;



		/*for outisde chose*/
		bool			_Writeok;
		bool			_Readok;
		bool			_modeChange;
		bool			_init;
		bool			_isEnd;

		void			canRead();
		void			canWrite();
		std::string		determinateType(const std::string &name_file);

		/*initialisation*/

		bool			_goodRequest;
		bool			checkRequest(const Server &server);

		/*Get methode*/

		std::string		LocalPathFile_get();
		void			getMethod(const Server &server);
		void			OpenFile_get(std::string &file_name);
		void			initialize_get();
		void			builRespondGet();
		bool			autoIndexRequired();

		bool			_boolAutoIndex;
		int				_file;
		std::string		_name_file;
		bool			_headerBuild;

		/*Post methode*/

		void			postMethod();
		int				_tmp_upload_fd;

		/*delete methode*/

		void			deleteMethod();

		/* Redirection management */

		bool			redirectionManage();
		bool			_RedirectionStart;

		/*Reponse Requete*/

		std::string		ErrorRespond(const Server &server);
		std::string		buildSimpleErrorResponse();
		bool			init_error_file(const std::string &error_page, std::string &errResp);
		std::string		_respond;
		HttpRequest		_request;
		bool			_tmpEnd;
};

#endif
