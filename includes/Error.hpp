#ifndef ERROR_HPP
#define ERROR_HPP

# include <string>
# include <map>
# include <fcntl.h>

class Error
{
	public :
		typedef std::map<int, std::string> ErrMap;

		Error();
		~Error();
		
		std::string getError(int nbr);
		
	private :
		static ErrMap _errMap;
		static void initMap();
		
};

#endif