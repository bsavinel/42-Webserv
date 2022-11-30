/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:55 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 13:32:57 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPMANAGER_HPP
#define HTTPMANAGER_HPP

class HttpManager;

# include <string>
# include "define.hpp"
# include "HttpRequest.hpp"
# include "Cgi.hpp"
# include <sys/types.h>
# include "Server.hpp"
# include <sys/socket.h>
#include <fstream>


std::string HeaderRespond (off_t contentLenght, int statusCode, std::string type = std::string(), std::vector<std::string> cookies = std::vector<std::string>());

typedef struct s_multipart_param
{
	std::pair<std::string, bool>	contentDisposition;
	std::pair<std::string, bool>	fileName;
	std::pair<std::string, bool>	contentType;
}	t_multipart_param;

typedef struct s_process
{
	bool	boundaryStart;
	bool	header;
	bool	body;
	bool	boundaryEnd;
}	t_process;

class HttpManager
{
	public:
		HttpManager(int sockClient);
		HttpManager(const HttpManager &rhs);
		~HttpManager();
		HttpManager &operator=(const HttpManager &rhs);

		void		initialize(const Server &server);
		bool		checkIfMethodIsAthorized();
		bool		checkRequest(const Server &server);
		std::string	determinateType(const std::string &name_file);
		bool		applyMethod(const Server &server);
		void		canRead();
		void		canWrite();
		int			receiver(void);
		void		sender();
		std::string ErrorRespond(const Server &server);
		std::string initDefaultErrorResponse();
		bool		init_error_file(const std::string &error_page, std::string &errResp, int error_code);
		void		manageCgi(HttpRequest &_request, const Server &server);

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

	void	methodPOSTfstream();
	std::ofstream		openUploadFilefstream();
std::string getFileName();
//	void parseMultiPart(std::fstream &fstream);
	int parseMultiPart(std::fstream &fstream);
	std::string			getUploadFileName( void );
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
	
		void	launchCgi();
		void	checkIfProcessIsFinished();
		void	fillResponseWithCgiOutput();




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

		// int				_tmp_upload_fd;
		// std::ifstream	_tmp_upload_i;
		std::ofstream	_tmp_upload_o;
		std::fstream	_tmp_upload;
		std::string 	_tmpFileName;
		/*delete methode*/
	
	private:
		void				methodPOST();
		int					_tmp_upload_fd;
				/* multipart */
		std::fstream		_uploaded;
		t_process			_process;
		bool				_new_process;
		t_multipart_param	_multipart_param;
		int					_lenOfRequestAlreadyRead;
		// ------------------------------------------------------
		// |				METHODS ---- DELETE					|
		// ------------------------------------------------------

	public:
		void methodDELETE();

		/* Redirection management */

		bool manageRedirection();
		bool _RedirectionStart;

		/*Reponse Requete*/

		// std::string		ErrorRespond(const Server &server);
		std::string		buildSimpleErrorResponse();
		// bool			init_error_file(const std::string &error_page, std::string &errResp);
		// std::string		_respond;
		// HttpRequest		_request;
		bool			_requestFullyReceive;
		// bool			_tmpEnd;
		int				_lenRead;


};

#endif
