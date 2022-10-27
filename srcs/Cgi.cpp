#include "Cgi.hpp"

Cgi::Cgi()
{

}

Cgi::~Cgi()
{
	//int i = 0;
	//int j = 0;

	/*while (_env[i])
		i++;
	while(j <= i)
		free(_env[j]);
	free(_env);*/
	
}

void Cgi::initialise_env(HttpRequest &request, const Server &server)
{
	std::cout << "IM INTO INITIALISZZZZ" << std::endl;
	std::vector<std::string>	env_var;

	env_var.push_back("SERVER_SOFTWARE=Webserv/1.0");
	env_var.push_back("SERVER_NAME=" + server.getServerName());
	env_var.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_var.push_back("SERVER_PROTOCOL=HTTP/1.1");
	env_var.push_back("SERVER_PORT=" + (std::string) ft_itoa(server.getPort()));
	env_var.push_back("REQUEST_METHOD=" + request.getMethod().first);
	env_var.push_back("PATH_INFO=" + buildLocalPath(request)); // the path to the script, example : methode/index.php
	env_var.push_back("PATH_TRANSLATED=" + buildLocalPath(request)); // idem 
	env_var.push_back("SCRIPT_NAME=" + buildLocalPath(request)); // the constructed path to the script /data/www/script.php
	env_var.push_back("SCRIPT_FILENAME=" + buildLocalPath(request)); // the constructed path to the script /data/www/script.php
	env_var.push_back("QUERY_STRING=" + request.getUrl().first);
	env_var.push_back("CONTENT_LENGTH=0");
	
	std::vector<std::string>::iterator itEnvVar = env_var.begin();


	std::cerr << "Before Malloc" << std::endl;
	std::cerr << "ENV VAR SIZE = " << env_var.size() << std::endl;
	std::cerr << "address : " << &_env << std::endl; 
	char **tmp = new char*[env_var.size() + 1];
	std::cerr << "APRES LE MALLOC = " << env_var.size() << std::endl;
	_env = tmp;
	int i = 0;
	while (i < (int) env_var.size())
	{
		_env[i] = strdup((*itEnvVar).c_str());
		itEnvVar++;
		i++;
	}
	_env[i] = NULL;
}

void	Cgi::printEnv() const
{
	int i  = 0;
	while (_env[i])
	{
		std::cout << _env[i] << std::endl;
		i++;
	}
}