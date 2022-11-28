/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:15 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:43:16 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : autoindex(false)
{
	return_code = 0;
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
		locate = rhs.locate;
	}
	return (*this);
}

Location::~Location()
{}



void Location::setConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted, std::string &loc)
{
	locate = loc;
	it+=2;
	while (it != splitted.end() && (*it).compare("}") != 0)
	{
		if ((*it).compare("allow") == 0)
		{
			if ((*(it + 1)) != ";")
				set_allowed_methods(it, splitted);
			else
				it++;
		}
		else if ((*it).compare("return") == 0 && (*(it + 1)) != ";")
			set_redirection(it);
		else if ((*it).compare("root") == 0 && (*(it + 1)) != ";" && (*(it + 2)) == ";")
			set_root_path(it);
		else if ((*it).compare("index") == 0 && (*(it + 1)) != ";" && (*(it + 2)) == ";")
			this->index_path = *++it;
		else if ((*it).compare("autoindex") == 0 && (*(it + 2)) == ";")
			set_autoindex(it);
		else if ((*it).compare("cgi_pass") == 0 && (*(it + 1)) != ";" && (*(it + 3)) == ";")
			set_cgi_pass(it);
		else if ((*it).compare("upload_store") == 0 && (*(it + 1)) != ";" && (*(it + 2)) == ";")
			set_upload_store(it);
		else if (*it != ";")
		{
			std::cout << "ERROR OPTION = " << *(it) << ", NEXT = " << *(it + 1) << std::endl;
			throw exceptWebserv ("Error Location : option not compatible or missing \';\'");
		}
		it++;
	}
}

// ------------------------------------------------------------------------------------------
// |										GETTERS											|
// ------------------------------------------------------------------------------------------


const std::vector<std::string> & Location::getAllowedMethods() const
{
	return(allowed_methods);
}


const int& Location::getReturnCode() const
{
	return(this->return_code);
}

const std::string& Location::getRedirectionPath() const
{
	return(this->redirection_path);
}

const std::string& Location::getRootPath() const
{
	return(this->root_path);
}

const std::string& Location::getIndexPath() const
{
	return(this->index_path);
}

const bool& Location::getAutoPath() const
{
	return(this->autoindex);

}
const std::string& Location::getCgiFileExtension() const
{
	return(this->cgi_file_extension);
}

const std::string& Location::getCgiPathToScript() const
{
	return(this->cgi_path_to_script);
}

const std::string& Location::getUploadDirectory() const
{
	return(this->upload_dir);
}

const std::string&	Location::getLocate() const
{
	return locate;
}


// ------------------------------------------------------------------------------------------
// |										PRINTS											|
// ------------------------------------------------------------------------------------------


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



// ------------------------------------------------------------------------------------------
// |									PRIVATE METHODS										|
// ------------------------------------------------------------------------------------------


void Location::set_allowed_methods(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted)
{
	it++;
	while (it != splitted.end() && (*it).compare(";") != 0)
		this->allowed_methods.push_back(*it++);
}

void	Location::check_param(std::vector<std::string>::iterator & it)
{
	std::string	param = *++it;

	std::string::iterator itParam = param.begin();
	while (itParam != param.end())
	{
		if(!isdigit(*itParam))
		{
			if(!is_file_path(param))
				throw exceptWebserv("Error Config : return value should be a path to a dir");
			this->redirection_path = param;
			return ;	
		}
		itParam++;
	}
	this->return_code = atoi(param.c_str());
}

void	Location::check_both_params(std::vector<std::string>::iterator & it)
{
	std::string	first_param = (*++it).c_str();
	std::string::iterator itFirstParam = first_param.begin();
	while (itFirstParam != first_param.end())
	{
		if(!isdigit(*itFirstParam))
				throw exceptWebserv("Error Config : redirection parameters should start with error_code and then redirection_path");
		itFirstParam++;
	}
	int tmp = atoi(first_param.c_str());
	if(tmp)
	{
		this->return_code = tmp;
		if(!check_existing_error_code(tmp))
			throw exceptWebserv("Error Config : return code does not exist");
		std::string path =  *++it;
		if(!is_file_path(path))
			throw exceptWebserv("Error Config : return value should be a path to a dir");
		this->redirection_path = path;
	}
	else
		this->redirection_path = *it;
}

void	Location::set_redirection(std::vector<std::string>::iterator & it)
{
	int nbr_parameters_redirection = 0;

	while (*(it + nbr_parameters_redirection + 1) != ";")
		nbr_parameters_redirection++;
	if(nbr_parameters_redirection == 1)
		check_param(it);
	else if(nbr_parameters_redirection == 2)
		check_both_params(it);
	else
		throw exceptWebserv("Error Config : redirection too many param");
}

void	Location::set_root_path(std::vector<std::string>::iterator & it)
{
	std::string path =  *++it;
	if(!is_dir_path(path))
		throw exceptWebserv("Error Config : root value should be a path to a dir");
	this->root_path = path;
}


void	Location::set_autoindex(std::vector<std::string>::iterator & it)
{
	if ((*++it).compare("on") == 0)
		this->autoindex = true;
	else if((*it).compare("off") == 0)
		this->autoindex = false;
	else
		throw exceptWebserv("Error Config : autoindex should set on \"on\" or \"off\"");
}

void	Location::set_cgi_pass(std::vector<std::string>::iterator & it)
{
	this->cgi_file_extension = *++it;
	if(!is_extension_file(cgi_file_extension))
		throw exceptWebserv("Error Config : cgi pass value should be an extension file starting with \'.\'");
	std::string path =  *++it;
	if(!is_file_path(path))
		throw exceptWebserv("Error Config : cgi path value should be a path to a binary file");
	this->cgi_path_to_script = path;
}

void	Location::set_upload_store(std::vector<std::string>::iterator & it)
{
	std::string path =  *++it;
	if(!is_dir_path(path))
		throw exceptWebserv("Error Config : upload_dir value should be a path to a dir");
	this->upload_dir = path;
}

// ------------------------------------------------------------------------------------------
// |										EXCEPTION										|
// ------------------------------------------------------------------------------------------

Location::exceptionLocation::exceptionLocation(const std::string content) throw()
{
	_content = content;
}

Location::exceptionLocation::~exceptionLocation() throw()
{

}

const char *Location::exceptionLocation::what() const throw()
{
	return _content.c_str();
}

