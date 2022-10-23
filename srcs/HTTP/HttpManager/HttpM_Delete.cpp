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
			_errorCode = 204;
		else
		{
			if (errno == ENOENT)
				_errorCode = 404;
			else if (errno == EINVAL || errno == ENAMETOOLONG || errno == ELOOP)
				_errorCode = 400;
			else
				_errorCode = 403;
		}
		_headerBuild = true;
	}
	else
		_isEnd = true;
}