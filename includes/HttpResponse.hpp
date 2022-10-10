#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

#include <string>
#include "define.hpp"
#include "HttpRequest.hpp"

class HttpResponse
{
	public:
		HttpResponse();
		HttpResponse(t_socket fd);
		HttpResponse(const HttpResponse &rhs);
		~HttpResponse();	
		HttpResponse &operator=(const HttpResponse &rhs);
		bool buildRespond(const HttpRequest &request, int maxOperation);

	private:
		bool		_initialized;
		t_socket	_fd;
		int			_file;
};

#endif