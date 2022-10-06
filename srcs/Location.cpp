#include "Location.hpp"

Location::Location()
{
}

Location::Location(const Location & src)
{
	*this = src;
}

Location & Location::operator=(const Location & rhs)
{
	if(this != &rhs)
	{
		this->allowed_methods = rhs.allowed_methods;
		this->return_code = rhs.return_code;
		this->redirection_path = rhs.redirection_path;
		this->root_path = rhs.root_path;
		this->index_path = rhs.index_path;
		this->autoindex = rhs.autoindex;
		this->cgi_file_extension = rhs.cgi_file_extension;
		this->cgi_path_to_script = rhs.cgi_path_to_script;
		this->upload_dir = rhs.upload_dir;
	}
	return (*this);
}

Location::~Location()
{}


void Location::getConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	while (it != splitted.end() && (*it).compare("}") != 0)
	{
		if ((*it).compare("allow") == 0)
		{
			it++;
			while (it != splitted.end() && (*it).compare(";") != 0)
				this->allowed_methods.push_back(*it++);
		}
		else if ((*it).compare("return") == 0)
		{
			this->return_code = atoi((*++it).c_str());
			this->redirection_path = *++it;
		}
		else if ((*it).compare("root") == 0)
			this->root_path = *++it;
		else if ((*it).compare("index") == 0)
			this->index_path = *++it;
		else if ((*it).compare("autoindex") == 0)
		{
			if ((*++it).compare("on") == 0)
				this->autoindex = true;
			else if((*it).compare("off") == 0)
				this->autoindex = false;
		}
		else if ((*it).compare("cgi_pass") == 0)
		{
			this->cgi_file_extension = *++it;
			this->cgi_path_to_script = *++it;
		}
		else if ((*it).compare("upload_store") == 0)
			this->upload_dir = *++it;
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
	if(!index_path.empty())
		std::cout <<"index_path : " << index_path << std::endl;
	std::cout <<"autoindex : " << autoindex << std::endl;
	std::cout <<"cgi_file_extension : " << cgi_file_extension << std::endl;
	std::cout <<"cgi_path_to_script : " << cgi_path_to_script << std::endl;
	std::cout <<"upload_dir : " << upload_dir << std::endl;
}