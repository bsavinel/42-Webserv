#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

#include "launcher.hpp"
#include "exceptWebserv.hpp"
#include "Config.hpp"
#include "Server.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		try
		{
			launcher(av[1]);
		}
		catch(const exceptWebserv& e)
		{
			std::cerr << e.what() << std::endl;
			if (errno)
				std::cerr << "Errno : " << strerror(errno) << std::endl;
			return(EXIT_FAILURE);
		}
	}
	else
		std::cout << "Missing configuration file" << std::endl;
	return(EXIT_SUCCESS);
}