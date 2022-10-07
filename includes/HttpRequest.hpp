#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>

class HttpRequest
{
	public :
		HttpRequest();
		HttpRequest(std::string client_request);
		HttpRequest(const HttpRequest & src);
		HttpRequest & operator=(const HttpRequest & rhs);
		~HttpRequest();

		void							parser(/*std::string &request*/);
		void							parseStartLine(std::string const & client_request);
		std::pair <std::string, bool>	parseHeader(std::string &header, std::string optionToFind);
		void							concatenate(char *str);
		void							erase(int index);

		std::string	getRequest(void) const; // GET POST DELETE

		std::pair<std::string, bool>	getMethod(void) const; // GET POST DELETE
		std::pair<std::string, bool>	getUrl(void) const; 
		std::pair<std::string, bool>	getHttpVersion(void) const; // Normallement on s'en branle

		std::pair<std::string, bool>	getConnection(void) const;
		std::pair<std::string, bool>	getAccept(void) const;
		std::pair<std::string, bool>	getSecFetchSite(void) const;
		std::pair<std::string, bool>	getSecFetchMode(void) const;
		std::pair<std::string, bool>	getSecFetchDest(void) const;
		std::pair<std::string, bool>	getReferer(void) const;
		std::pair<std::string, bool>	getAcceptEncoding(void) const;
		std::pair<std::string, bool>	getDnt(void) const;

		void							setRequest(std::string const & request);

	private :
		std::string					_request;
		// Start line
		std::string					_startLine;

		std::pair<std::string, bool> _method; // GET POST DELETE
		std::pair<std::string, bool> _url;
		std::pair<std::string, bool> _httpVersion; // Normallement on s'en branle

		std::pair<std::string, bool> _Connection;
		std::pair<std::string, bool> _Accept;
		std::pair<std::string, bool> _SecFetchSite;
		std::pair<std::string, bool> _SecFetchMode;
		std::pair<std::string, bool> _SecFetchDest;
		std::pair<std::string, bool> _Referer;
		std::pair<std::string, bool> _AcceptEncoding;
		std::pair<std::string, bool> _dnt;
		//std::string _AcceptLanguage;
		//std::string _sec_ch_ua;
		//std::string _sec_ch_ua_mobile;
		//std::string _UserAgent;
		//std::string _sec_ch_ua_platform;
		//std::string _Host;
};

std::ostream &	operator<<( std::ostream & o, HttpRequest const & rhs);

#endif
