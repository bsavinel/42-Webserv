#include "Cgi.hpp"

Cgi::Cgi()
{

}

Cgi::~Cgi()
{

}

void Cgi::initialise_env(HttpManager &manager, const Server &server)
{
	_env.insert("SERVER_SOFTWARE=");
	_env.insert("SERVER_NAME=" + server.getServerName());
	_env.insert("GATEWAY_INTERFACE=CGI/1.1");
	_env.insert("SERVER_PROTOCOL=HTTP/1.1");
	_env.insert("SERVER_PORT=" + ft_itoa(server.getPort()));
	_env.insert("REQUEST_METHOD=" + manager.getRequest().getMethod().first);
	_env.insert("PATH_INFO=" + manager.getRequest().getUrl()); // the path to the script, example : methode/index.php
	_env.insert("PATH_TRANSLATED=" + manager.getRequest().getUrl()); // idem 
	//_env.insert("SCRIPT_NAME=" + ) // the constructed path to the script /data/www/script.php
	_env.insert("QUERY_STRING=" + )


}