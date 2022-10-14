#ifndef EXECPTWEBSERV_HPP
# define EXECPTWEBSERV_HPP

# include <exception>
# include <string>

class exceptWebserv: public std::exception
{
	public:
		exceptWebserv(const std::string content) throw();
		const char *what() const throw();
		~exceptWebserv() throw();

	private:
		std::string _content;
};

#endif
