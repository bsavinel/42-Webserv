#include "Error.hpp"

Error::Error()
{
	if(is_init == 0)
	{
		is_init = 1;
		errors.insert(std::make_pair(100,"Continue"));
		errors.insert(std::make_pair(101,"Switching Protocols"));
		errors.insert(std::make_pair(200,"OK"));
		errors.insert(std::make_pair(201,"Created"));
		errors.insert(std::make_pair(202,"Accepted"));
		errors.insert(std::make_pair(203,"Non-Authoritative Information"));
		errors.insert(std::make_pair(204,"No Content"));
		errors.insert(std::make_pair(205,"Reset Content"));
		errors.insert(std::make_pair(206,"Reset Content"));
	}
}

Error::~Error()
{

}

std::string Error::getError(int nbr)
{
	return (errors.find(nbr)->second);
}

int Error::is_init = 0;


