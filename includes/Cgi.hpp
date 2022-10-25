#ifndef CGI_HPP
#define CGI_HPP

# include <iostream>
# include <string>
# include <vector>
# include <HttpManager.hpp>
# include <HttpRequest.hpp>
# include <stdlib.h>

class Cgi
{
	private :
		typedef std::vector<std::string> envVec;

		envVec		_env;
		std::string	_request;

	public :
		Cgi();
		~Cgi();

		void	initialise_env(HttpManager &manager, const &Server);
};


#endif