#ifndef CGI_HPP
#define CGI_HPP

# include <iostream>
# include <string>
# include <vector>
# include <HttpManager.hpp>
# include <HttpRequest.hpp>
# include <stdlib.h>

class HttpManager;

class Cgi
{
	private :

		char*		_env[];
		std::string	_request;

	public :
		Cgi();
		~Cgi();

		void	initialise_env(HttpManager &manager, const Server &server);
		void	printEnv() const;
};


#endif