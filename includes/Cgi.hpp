#ifndef CGI_HPP
#define CGI_HPP

class Cgi;

# include <iostream>
# include <string>
# include <vector>
# include "HttpRequest.hpp"
# include <stdlib.h>
# include "utils.hpp"

class Cgi
{
	private :

		char **		_arg;
		std::string	_path_to_script;
		std::string	_exec;
		char**		_env;
		std::string	_request;

	public :
		Cgi();
		~Cgi();

		void	initialise_env(HttpRequest &manager, const Server &server);
		void	set_argv();
		void	printEnv() const;
		void	printArg() const;
		void	execute();

};


#endif