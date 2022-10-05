#include "launcher.hpp"
#include <stdlib.h>
#include "exceptWebserv.hpp"
#include <iostream>
#include <errno.h>
#include <string.h>


#include "Config.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		try
		{
			Config configuration(av[1]);
			configuration.print_all_conf();
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