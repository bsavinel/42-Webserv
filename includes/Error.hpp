#ifndef ERROR_HPP
#define ERROR_HPP

# include <string>
# include <map>
# include <fcntl.h>
# include "exceptWebserv.hpp"

std::string buildErrorPage(int err);

class Error
{
	public :
		typedef std::map<int, std::string> ErrMap;

		Error();
		~Error();
		
		std::string getError(int nbr);
		
	private :
		static ErrMap _errMap;
		static int init;
		static void initMap();
		
};

#endif