#include "Location.hpp"

Location::Location()
{
	std::cout << "Location default constructor called" << std::endl;
}

Location::Location(const Location & src)
{
	*this = src;
	std::cout << "Location copy constructor called" << std::endl;
}

Location & Location::operator=(const Location & rhs)
{
	if(this != &rhs)
	{
		
	}
	std::cout << "Location assignment constructor called" << std::endl;
	return (*this);
}

Location::~Location()
{
	std::cout << "Location default destructor called" << std::endl;
}


void Location::getConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	while (it != splitted.end() && (*it).compare("}") != 0)
	{

		if ((*it).compare("allow") == 0)
		{
			it++;
			while (it != splitted.end() && (*it).compare(";") != 0)
			{
				this->allowed_methods.push_back(*it);
				it++;
			}
		}
		else if ((*it).compare("return") == 0)
		{
			it++;
			this->return_code = atoi((*it).c_str());
			it++;
			this->redirection_path = *it;
		}
		else if ((*it).compare("root") == 0)
		{
			it++;
			this->root_path = *it;
		}
		else if ((*it).compare("index") == 0)
		{
			it++;
			this->index_path = *it;
		}
		else if ((*it).compare("autoindex") == 0)
		{
			it++;
			if ((*it).compare("on") == 0)
				this->autoindex = true;
			else
				this->autoindex = false;
		}
		else if ((*it).compare("cgi_pass") == 0)
		{
			it++;
			this->cgi_file_extension = *it;
			it++;
			this->cgi_path_to_script = *it;
		}
		else if ((*it).compare("upload_store") == 0)
		{
			it++;
			this->upload_dir = *it;
			
		}
		it++;
	}
}

void	Location::printConfig()
{
	std::cout << "Allowed methods : ";
	for (std::vector<std::string>::iterator beg = allowed_methods.begin(); beg != allowed_methods.end(); beg++)
		std::cout << *beg << " ";
	std::cout << std::endl;
	std::cout <<"Return code : " << return_code << std::endl;
	std::cout <<"Redirection path : " << redirection_path << std::endl;
	std::cout <<"root_path : " << root_path << std::endl;
	std::cout <<"index_path : " << index_path << std::endl;
	std::cout <<"autoindex : " << autoindex << std::endl;
	std::cout <<"cgi_file_extension : " << cgi_file_extension << std::endl;
	std::cout <<"cgi_path_to_script : " << cgi_path_to_script << std::endl;
	std::cout <<"upload_dir : " << upload_dir << std::endl;
}