#ifndef HTTPMANAGER_HPP
#define HTTPMANAGER_HPP

class HttpManager;

#include <string>
#include "define.hpp"
#include "define.hpp"
#include "HttpRequest.hpp"
#include "Cgi.hpp"
#include <sys/types.h>
#include "Server.hpp"
#include <sys/socket.h>

std::string HeaderRespond(off_t contentLenght, int statusCode, std::string type = std::string(), std::vector<std::string> cookies = std::vector<std::string>());

class HttpManager
{
	public:
		HttpManager(int sockClient);
		HttpManager(const HttpManager &rhs);
		~HttpManager();
		HttpManager &operator=(const HttpManager &rhs);

		void		initialize(const Server &server);
		bool		checkIfMethodIsAthorized();
		bool		applyMethod(const Server &server);
		void		canRead();
		void		canWrite();
		int			receiver(void);
		void		sender();
		bool		checkRequest(const Server &server);
		std::string	determinateType(const std::string &name_file);
		std::string ErrorRespond(const Server &server);
		std::string initDefaultErrorResponse();
		bool		init_error_file(const std::string &error_page, std::string &errResp);
		void		launch_cgi(HttpRequest &_request, const Server &server);

		// ----------------------
		// |		GETTERS		|
		// ----------------------

		const bool			&getInit() const;
		const bool			&getWriteOk() const;
		const bool			&getReadOk() const;
		const bool			&getIsEnd() const;
		const bool			&getModeChange() const;
		const HttpRequest	&getRequest() const;
		const std::string	&getResponse() const;
		const HttpRequest	&getClassRequest() const;

		// ----------------------
		// |		SETTERS		|
		// ----------------------

		void 				setModeChange(bool modeChange);
		void 				setErrorCode(int statusCode);
		void 				setIsEnd(bool IsEnd);
		void 				setInit(bool init);

	private:
		bool		_Writeok;
		bool		_Readok;
		bool		_modeChange;
		bool		_init;
		bool		_tmpEnd;
		bool		_isEnd;
		bool		_goodRequest;
		int			_errorCode;
		t_socket	_socketClient;
		Cgi			_cgi;
		std::string _respond;
		HttpRequest _request;



		// ------------------------------------------------------
		// |				METHODS ---- CGI					|
		// ------------------------------------------------------

	private:
		
		bool 	_firstPassage;
		bool	_proccess_fini;


		// ------------------------------------------------------
		// |				METHODS ---- GET					|
		// ------------------------------------------------------

	public:
		std::string		retrieveCorrespondingLocalPath();
		void			methodGET(const Server &server);
		void			openFileGET(std::string &file_name);
		void			initGET();
		void			buildGetRespond();
		bool			autoIndexRequired();

	private :

		int				_file_fd;
		std::string 	_name_file;
		bool			_boolAutoIndex;
		bool			_headerBuild;

		// ------------------------------------------------------
		// |				METHODS ---- POST					|
		// ------------------------------------------------------

	public :
		void methodPOST();
	
	private:
		int _tmp_upload_fd;

		// ------------------------------------------------------
		// |				METHODS ---- DELETE					|
		// ------------------------------------------------------

	public:
		void methodDELETE();




		/* Redirection management */

		bool manageRedirection();
		bool _RedirectionStart;

		/*Reponse Requete*/	
};

#endif
