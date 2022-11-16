#include "Config.hpp"

Config::Config()
{

}

void Config::look_for_and_initialise_server_block(std::vector<std::string>::iterator *beg, std::vector<std::string> splitted)
{
	if ((**beg).compare("server") == 0 && (*(*beg + 1)).compare("{") == 0)
	{
		if(checkbrackets(++*beg, splitted))
		{
			Server *server = getServerToken(*beg, splitted);
			if(!server)
				throw exceptWebserv("Error Server : cannot initialize the server");
			this->servers.push_back(server);
		}
	}
}



void Config::init(char *config_file)
{
	std::string content_file = read_file(config_file);
	std::string delimiter(" \t;{}");
	remove_comment(content_file);
	remove_nl(content_file);
	std::vector<std::string> splitted = split_vector(content_file, delimiter);
	for(std::vector<std::string>::iterator beg = splitted.begin(); beg != splitted.end(); beg++)
	{
		// look_for_and_initialise_server_block(&beg, splitted);
		if ((*beg).compare("server") == 0 && (*(beg + 1)).compare("{") == 0)
		{
			if(checkbrackets(++beg, splitted))
			{
				Server *server = getServerToken(beg, splitted);
				if(!server)
					throw exceptWebserv("Error Server : cannot initialize the server");
				this->servers.push_back(server);
			}
		}
	}
	checkLocBlock();
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
	int nbrserv = servers.size();
	std::cout << "NBR DE SERVER" << nbrserv << std::endl;
	for (std::list<Server*>::iterator it = this->servers.begin(); it != this->servers.end(); it++)
	{	
		if(*it)
			delete *it;
	}
}

	// ------------------------------------------------------------------------------------------
	// |										GETTERS											|
	// ------------------------------------------------------------------------------------------



std::list<Server*> & Config::getServersList()
{
	return (servers);
}






	// ------------------------------------------------------------------------------------------
	// |										PRINTS											|
	// ------------------------------------------------------------------------------------------



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







	// ------------------------------------------------------------------------------------------
	// |									PRIVATE METHODS										|
	// ------------------------------------------------------------------------------------------

bool	is_allowed_method(Location *locBlock)
{
	std::vector<std::string> vec = locBlock->getAllowedMethods();
	std::vector<std::string>::iterator it = vec.begin();
	std::vector<std::string>::iterator ite =vec.end();
	while(it != ite)
	{
		if((*it).compare("GET") != 0 && (*it).compare("POST") != 0 && (*it).compare("DELETE") != 0)
			return (0);
		it++;
	}
	return (1);
}


bool	check_loc_path(std::string path)
{
	if(path.rfind("/", 0) == std::string::npos)
		return (0);
	std::string::iterator lastchar= path.end() - 1;
	if((*lastchar) != '/')
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
			if(!check_loc_path(itLoc->first))
				throw exceptWebserv("Error Config : Location block path should start and finish with '/'");
			if(!is_allowed_method(itLoc->second))
				throw exceptWebserv("Error Config : Method not recognized, check the allow option");
		}
	}
	return (1);
}

Server* Config::getServerToken(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	Server *server = new Server();

	try
	{
		server->setConfig(it, splitted);
	}
	catch(const exceptWebserv& e)
	{
		std::cerr << e.what() << std::endl;
		delete server;
		server = NULL;
		if (errno)
			std::cerr << "Errno : " << strerror(errno) << std::endl;
		return(NULL);
	}
	
	return(server);
}

