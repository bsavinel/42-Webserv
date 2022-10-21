#include "HttpManager.hpp"
#include <iostream>
#include <stdio.h>

void	HttpManager::deleteMethod()
{
	std::string	path_to_remove;
	int			exit_code;

	canWrite();
	if (_headerBuild == false)
	{
		path_to_remove = _request.getLocation()->getRootPath();
		path_to_remove.erase(--path_to_remove.end());
		path_to_remove += _request.getUrl().first;
		exit_code = remove(path_to_remove.c_str());
		std::cout << "Path to remove: " << path_to_remove << std::endl;
		if (exit_code == 0)
			_respond = "HTTP/1.1 204 No Content\n\n";
		else
		{
			if (errno == ENOENT)
				_respond = "HTTP/1.1 404 Not Found\n\n";
			else if (errno == EINVAL || errno == ENAMETOOLONG || errno == ELOOP)
				_respond = "HTTP/1.1 400 Bad Request\n\n";
			else
				_respond = "HTTP/1.1 403 Forbidden\n\n";
		}
		_headerBuild = true;
	}
	else
		_isEnd = true;
}