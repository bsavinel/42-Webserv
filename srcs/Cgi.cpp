#include "Cgi.hpp"
#include "define.hpp"
#include <signal.h>

Cgi::Cgi()
{
}

Cgi::~Cgi()
{
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
	env_var.push_back("REQUEST_METHOD=" + request.methodGET().first);
	env_var.push_back("PATH_INFO=" + buildLocalPath(request)); // the path to the script, example : methode/index.php
	env_var.push_back("PATH_TRANSLATED=" + buildLocalPath(request)); // idem 
	env_var.push_back("SCRIPT_NAME=" + buildLocalPath(request)); // the constructed path to the script /data/www/script.php
	env_var.push_back("SCRIPT_FILENAME=" + buildLocalPath(request)); // the constructed path to the script /data/www/script.php
	env_var.push_back("QUERY_STRING=" + request.getUrl().first);
	env_var.push_back("CONTENT_LENGTH=0");
	env_var.push_back("REDIRECT_STATUS=200");
	env_var.push_back("HTTP_COOKIE=" + request.getCookie().first);

	
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

void	Cgi::printArg() const
{
	int i  = 0;
	while (_arg[i])
	{
		std::cout << _arg[i] << std::endl;
		i++;
	}
}

void	Cgi::set_argv()
{
	std::vector<std::string> executable;

	executable.push_back(retrieve_from_left_till_char(_exec, '/'));
	executable.push_back(_path_to_script);

	std::vector<std::string>::iterator itEnvVar = executable.begin();
	_arg = new char*[executable.size() + 1];
	int i = 0;
	while (i < (int) executable.size())
	{
		_arg[i] = strdup((*itEnvVar).c_str());
		itEnvVar++;
		i++;
	}
	_arg[i] = NULL;
}

void Cgi::execute()
{
	if(pipe(_pip) == -1)
		throw exceptWebserv("Error CGI : failed to create a pipe");
	if((_pid = fork()) == -1)
		throw exceptWebserv("Error CGI : failed to fork");
	if(_pid == 0) 
	{
		//Child Process, CGI execution
		dup2(_pip[1], STDOUT_FILENO);
		close(_pip[0]);
		close(_pip[1]);
		execve(_exec.c_str(), _arg, _env);
	}
	else
	{
		_start = give_time();
		close(_pip[1]);
	}
}

/*retour 0 process non fini
 retour 1 process fini
 retour -1 process timeout*/

int	Cgi::feedOutput()
{
	char	buff[LEN_TO_READ + 1];
	int		nbytes;

	if (give_time() > _start + 15)
	{
		kill(_pid, SIGKILL);
		return -1;
	}
	memset(buff, 0, LEN_TO_READ);
	nbytes = read(_pip[0], buff, LEN_TO_READ);
	_output.insert(_output.size(), buff, nbytes);
	if(nbytes == -1)
		throw exceptWebserv("Error CGI : failed to read output");
	if (waitpid(_pid, NULL, WNOHANG) == _pid)
	{
		memset(buff, 0, LEN_TO_READ);
		while((nbytes = read(_pip[0], buff, LEN_TO_READ) > 0))
		{
			_output.insert(0, buff, nbytes);
			memset(buff, 0, 4096);
		}
		store_cookies();
		manage_output();
		close(_pip[0]);
		return 1;
	}
	return 0;
}


void	Cgi::store_cookies()
{
	std::string	line;
	int i = 1;
	int	start_position_of_line = 0;
	int end_position_of_line = _output.find("\r\n");
	int end_of_header = _output.find("\r\n\r\n");

	while (end_position_of_line < end_of_header)
	{
		line = _output.substr(start_position_of_line, end_position_of_line - start_position_of_line);		
		if(line.find("Set-Cookie:") != std::string::npos)
			_cookies.push_back(line);
		start_position_of_line = end_position_of_line + 2;
		end_position_of_line = _output.find("\r\n", start_position_of_line);
		if(end_position_of_line >= end_of_header)
			end_position_of_line = end_of_header;
		i++;
	}
	std::cout<< std::endl<< std::endl<< std::endl;
}

void	Cgi::manage_output()
{
	size_t	ret;

	if ((ret = _output.find("\r\n\r\n")) != std::string::npos) 
		_output.erase(0, ret);
}

const std::string&	Cgi::getOutput() const
{	
	return(_output);
}

void	Cgi::set_path_cgi(std::string path)
{
	_exec = path;
}

const std::string&	Cgi::getScriptPath() const
{
	return(_path_to_script);
}

void	Cgi::cutOutput(int len)
{
	_output.erase(0, len);
}

const std::vector<std::string>	& Cgi::getCookies() const
{
	return(_cookies);
}

void	Cgi::free_argenv()
{
	int i = 0;

	while(_arg[i])
	{
		free(_arg[i]);
	}
	free(_arg);
	while(_env[i])
	{
		free(_env[i]);
	}
	free(_env);
}
