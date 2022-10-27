#ifndef CGI_HPP
#define CGI_HPP

class Cgi;

# include <iostream>
# include <string>
# include <vector>
# include "HttpRequest.hpp"
# include <stdlib.h>

class Cgi
{
	private :

		char**		_env;
		std::string	_request;

	public :
		Cgi();
		~Cgi();

		void	initialise_env(HttpRequest &manager, const Server &server);
		void	printEnv() const;
};


#endif