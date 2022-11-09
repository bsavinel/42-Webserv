#ifndef CGI_HPP
#define CGI_HPP

class Cgi;

# include <iostream>
# include <string>
# include <vector>
# include <ctime>
# include <stdlib.h>
# include <sys/wait.h>
# include "HttpRequest.hpp"
# include "utils.hpp"

class Cgi
{
	private :

		char **		_arg;
		std::string	_path_to_script;
		std::string	_exec;
		char**		_env;
		std::string	_request;
		std::string	_output;
		size_t		_start;
		time_t		_end;
		pid_t		_pid;
		int			_pip[2];


	public :
		Cgi();
		~Cgi();

		int					feedOutput();
		void				initialise_env(HttpRequest &manager, const Server &server);
		void				set_path_cgi(std::string path);
		void				set_argv();
		void				printEnv() const;
		void				printArg() const;
		const				std::string	&getOutput() const;
		const				std::string &getScriptPath() const;
		void				execute();
		void				manage_output();
		void				cutOutput(int len);

};


#endif