#include "Config.hpp"

static Server* getServerToken(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	Server *server = new Server();
	server->setConfig(it, splitted);
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
	if(!this->checkLocBlock())
		throw exceptWebserv("Error Config : Location block path should be a directory");
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
	for (std::list<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
		delete *it;
}

//----------------------------MEMBER FUNCTION--------------------------

void	Config::print_all_conf()
{
	int i  = 0;
	if(this->servers.empty())
		throw exceptWebserv("Error config : no congiguration found");
	for(std::list<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{	
		std::cout << "------CONFIG " << ++i <<"------" << std::endl;
		(*it)->printConfig();
	}
}

//----------------------------GETTERS--------------------------

std::list<Server*> & Config::getServersList()
{
	return (servers);
}

bool	is_dir_path(std::string path)
{
	if(path.rfind("/", 0) == std::string::npos)
		return (0);
	std::string::iterator lastchar= path.end() - 1;
	if((*lastchar) != '/' )
		return(0);
	else
		return(1);
	
}

bool	Config::checkLocBlock()
{
	for (std::list<Server*>::iterator itLstServer = this->getServersList().begin();
		itLstServer != this->getServersList().end(); itLstServer++)
	{
		std::map<std::string, Location*> mapLocation = (*itLstServer)->getLocationsMap();
		for(std::map<std::string, Location*>::iterator itLoc = mapLocation.begin();
			itLoc != mapLocation.end(); itLoc++)
		{
			if(!is_dir_path(itLoc->first))
				return(0);
		}
	}
	return (1);
}