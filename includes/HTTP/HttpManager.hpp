#ifndef HTTPMANAGER_HPP
# define HTTPMANAGER_HPP

# include <string>
# include "define.hpp"
# include "define.hpp"
# include "HttpRequest.hpp"
#include <sys/types.h>
#include "Server.hpp"
#include <sys/socket.h>
#include <fstream>
// TODO metre trois if pour repartir dans la methode adequate
// TODO changer dans epoll le map des clients, pour lie au manager le fd
// TODO mettre class Socket dans manager
// TODO changer ServeurEvent et ClientEvent
// TODO lie le serveur au fd dans socket

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

	void	postMethodfstream();
	std::ofstream		openUploadFilefstream();
std::string getFileName();
	void parseMultiPart(std::fstream &fstream);
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
		std::string 	buildHeader(off_t contentLenght, int statusCode);

		/*initialisation*/



		/*Get methode*/

		std::string		LocalPathFile_get();
		void			getMethod();
		void			OpenFile_get(std::string &file_name);
		void			initialize_get();
		void 			buildHeaderGet();
		void			builRespondGet();

		int				_file;
		std::string		_name_file;
		bool			_headerBuild;

		/*Post methode*/

		void			postMethod();
		int				_tmp_upload_fd;
		std::ifstream	_tmp_upload_i;
		std::ofstream	_tmp_upload_o;
		std::fstream	_tmp_upload;
		std::string 	_tmpFileName;
		/*delete methode*/

		void			deleteMethod();

		/*Reponse Requete*/

		std::string		_respond;
		HttpRequest		_request;
		bool			_requestFullyReceive;
		bool			_tmpEnd;
		int				_lenRead;

		/* multipart */
		std::fstream	_uploaded;
		t_process		_process;
		bool			_new_process;
};

#endif
