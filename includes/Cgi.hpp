#ifndef CGI_HPP
#define CGI_HPP

class Cgi;

# include <iostream>
# include <string>
# include <vector>
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

	public :
		Cgi();
		~Cgi();

		void	initialise_env(HttpRequest &manager, const Server &server);
		void	set_argv();
		void	printEnv() const;
		void	printArg() const;
		const	std::string	&getOutput() const;
		void	execute();

};


#endif