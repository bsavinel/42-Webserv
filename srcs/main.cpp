#include "Server.hpp"
#include <cstdlib>

int main(int ac, char **av)
{
	(void)ac;
	(void)av;

	Server server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 100);
	try
	{
		server.launch();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		if (errno)
			std::cerr << "Errno : " << strerror(errno) << std::endl;
		return(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
