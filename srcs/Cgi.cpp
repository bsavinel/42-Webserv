#include "Cgi.hpp"

Cgi::Cgi()
{
	_exec = "/usr/bin/php8.1";
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
	std::vector<std::string>	env_var;
	
	_request = request.getRequest();
	_path_to_script = buildLocalPath(request);

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

	_env = new char*[env_var.size() + 1];
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

void	Cgi::set_argv()
{
	
	std::cout << retrieve_from_left_till_char(_exec, '/') << std::endl;


}

void Cgi::execute()
{
	execve(_exec.c_str(), _arg, _env);
}