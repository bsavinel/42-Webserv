#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>

class HttpRequest
{
	private :
		// Start line
		std::string _method; // GET POST DELETE
		std::string _url; 
		std::string _httpVersion;

	public :
		HttpRequest(char *);
		HttpRequest(const HttpRequest & src);
		HttpRequest & operator=(const HttpRequest & rhs);
		~HttpRequest();
};

#endif
/*
	private :
		std::string	_method; // GET POST DELETE
		std::string	_protocol; // HTTP 1.1
		std::string	_accept; // Accept: text/html, application/xml
		std::string _acceptCharset; // Accept-Charset: utf-8
		std::string _acceptEncoding; // Accept-Encoding: gzip
		std::string _acceptLanguage; // Accept-Language: fr-FR
		std::string _authorization; // Basic WjbU7D25zTAlV2tZ7==
		std::string _cacheControl; // Cache-Control: no-cache
		std::string _cookie; // Cookie: $Version=1; Content=23
		std::string _contentLength; // Content-Length: 212
		std::string _contentType; // Content-Type: application/x_222-form-urlencoded
		std::string _date; // Date: Mon, 9 March 2020 09:02:22 GMT
		std::string _expect; // Expect: 100-continue (le serveur doit envoyer le code 100 lorsqu’il est prêt à recevoir la requête)
		std::string _host; // Host: exemple.fr
		std::string _ifMatch; //If-Match: „ft678iujhnjio90’pöl“
		std::string _ifModifiedSince; // IF-Modified-Since: Mon 2 Mar 2020 1:00:00 GMT
		std::string _ifNoneMatch; // If-None-Match: „cxdrt5678iujhgbvb“
		std::string _ifRange; // If-Range: Mon 2 Mar 2020 1:00:00 GMT
		std::string _ifUnmodifiedSince; // If-Modified-Since: Mon 2 Mar 2020 1:00:00 GMT
		std::string _maxForward; // Max-Forwards: 12
		std::string _proxyAuthorization; // Proxy-Authorization: Basic WjbU7D25zTAlV2tZ7==
		std::string _range; // Range: bytes=0-9999
		std::string _referrer; // Referrer: https://exemple.fr/index.html
		std::string _TE; // TE: gzip, deflate
		std::string _userAgent; // Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.132 Safari/537.36
*/