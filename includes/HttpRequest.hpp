#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>

class HttpRequest
{
	public :
		HttpRequest();
		HttpRequest(char *client_request);
		HttpRequest(const HttpRequest & src);
		HttpRequest & operator=(const HttpRequest & rhs);
		~HttpRequest();

		void	parser(char *client_request);
		void	parseStartLine(std::string const & client_request);
		void	parseHeader(std::string &header, std::string &optionToFind, std::string &option);

	private :
		// Start line
		std::string _startLine;
		std::string _method; // GET POST DELETE
		std::string _url; 
		std::string _httpVersion; // Normallement on s'en branle

		std::string _Connection;
		std::string _Accept;
		std::string _SecFetchSite;
		std::string _SecFetchMode;
		std::string _SecFetchDest;
		std::string _Referer;
		std::string _AcceptEncoding;
		std::string _dnt;
		//std::string _AcceptLanguage;
		//std::string _sec_ch_ua;
		//std::string _sec_ch_ua_mobile;
		//std::string _UserAgent;
		//std::string _sec_ch_ua_platform;
		//std::string _Host;
		
};

#endif
