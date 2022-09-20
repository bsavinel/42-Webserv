#include "body.hpp"
#include <stdlib.h>
#include "exceptWebserv.hpp"
#include <iostream>
#include <errno.h>
#include <string.h>

int main(int ac, char **av)
{
	if (ac != 2)
		return 1;
	// TODO changer le catch
	try
	{
		starter(av);
	}
	catch(const exceptWebserv& e)
	{
		std::cerr << e.what() << std::endl;
		if (errno)
			std::cerr << "Errno : " << strerror(errno) << std::endl;
		return(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
