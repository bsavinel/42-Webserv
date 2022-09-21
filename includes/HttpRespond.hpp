#ifndef HTTPRESPOND_HPP
# define HTTPRESPOND_HPP

#include <string>

class HttpRespond
{
	public:
		HttpRespond();
		HttpRespond(const HttpRespond& rhs);
		~HttpRespond();
		
		HttpRespond &operator=(const HttpRespond& rhs);
		const std::string getContent() const;
		HttpRespond(const std::string content, const std::string contentType);
		std::string getHeader() const;
		std::string getHttpRespond() const;
		void setContent(const std::string content);
		void setContentType(const std::string contentType);
	
	private:
		std::string _contentType;
		std::string _content;
};

#endif