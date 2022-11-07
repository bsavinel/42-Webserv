#include "HttpManager.hpp"

bool HttpManager::redirectionManage()
{
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	if (_request.getLocation()->getRedirectionPath().empty())
		return false;
	if (_RedirectionStart == false)
	{
		_RedirectionStart = true;
		_respond.clear();
		if (!init_error_file(_request.getLocation()->getRedirectionPath(), _respond))
			return false;
	}
	else
	{
		nb_char = read(_file, buffer, LEN_TO_READ);
		if (nb_char > 0)
			_respond.insert(_respond.size(), &buffer[0], nb_char);
		if (nb_char < LEN_TO_READ)
		{
			close(_file);
			_isEnd = true;
		}
	}
	return true;
}