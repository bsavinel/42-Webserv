#include "Config.hpp"

static Server_config *getServerToken(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	Server_config *server = new Server_config();
	server->getConfig(it, splitted);
	return(server);
}

Config::Config(char *config_file)
{
	std::string content_file = read_file(config_file);
	std::string delimiter(" \t;{}");
	remove_comment(content_file);
	remove_nl(content_file);
	std::vector<std::string> splitted = split_vector(content_file, delimiter);
	for(std::vector<std::string>::iterator beg = splitted.begin(); beg != splitted.end(); beg++)
	{
		if ((*beg).compare("server") == 0 && (*(beg + 1)).compare("{") == 0)
		{
			if(checkbrackets(++beg, splitted))
				this->servers.push_back(getServerToken(beg, splitted));
		}
	}
}

Config::Config(const Config & src)
{
	*this = src;
}

Config & Config::operator=(const Config & rhs)
{
	if(this != &rhs)
		this->servers = rhs.servers;
	return(*this);
}

Config::~Config()
{
	for (std::list<Server_config*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
		delete *it;
}



//----------------------------MEMBER FUNCTION--------------------------

void	Config::print_all_conf()
{
	int i  = 0;
	if(this->servers.empty())
		throw exceptWebserv("Error config : no congiguration found");
	for(std::list<Server_config*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{	std::cout << "------CONFIG " << i <<"------" << std::endl;
		(*it)->printConfig();
		i++;
	}
}