#ifndef ERROR_HPP
#define ERROR_HPP

# include <string>
# include <map>
# include <fcntl.h>

class Error
{
	private :
		std::map<int, std::string> errors;
		static int is_init;
	
	public :
		Error();
		~Error();
		std::string getError(int nbr);

};

#endif