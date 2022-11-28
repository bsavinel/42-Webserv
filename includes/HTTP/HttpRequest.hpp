/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:57 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:58 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>

#include <Server.hpp>


class HttpRequest
{
	public :
		void							parser( void );
		void							parseStartLine(std::string const & client_request);
		std::pair <std::string, bool>	parseHeader(std::string &header, std::string optionToFind);
		void							concatenate(char *str);
		void							concatenateInsert(char *str, int len);
		void							erase(int index);

		std::string						&getRequest(void);
		std::pair<std::string, bool>	getMethod(void) const;
		std::pair<std::string, bool>	getUrl(void) const; 
		std::pair<std::string, bool>	getHttpVersion(void) const;

		std::pair<std::string, bool>	getConnection(void) const;
		std::pair<std::string, bool>	getAccept(void) const;
		std::pair<std::string, bool>	getSecFetchSite(void) const;
		std::pair<std::string, bool>	getSecFetchMode(void) const;
		std::pair<std::string, bool>	getSecFetchDest(void) const;
		std::pair<std::string, bool>	getReferer(void) const;
		std::pair<std::string, bool>	getAcceptEncoding(void) const;
		std::pair<std::string, bool>	getDnt(void) const;
		std::pair<std::string, bool>	getContentType(void) const;
		std::pair<std::string, bool>	getBoundary(void) const;
		std::pair<std::string, bool>	getCookie(void) const;
		std::pair<int, bool>			getContentLength(void) const;

		Location						*getLocation(void) const;
		void							setRequest(std::string const & request);
		void							setUrl(std::string const & url);
		void							setLocation(Location *location);

		Location						*findLocation(const Server &server);

	private :
		std::string						_request;

		std::string						_startLine;

		std::pair<std::string, bool>	_method;
		std::pair<std::string, bool> 	_url;
		std::pair<std::string, bool> 	_httpVersion;


		std::pair<std::string, bool> 	_Connection;
		std::pair<std::string, bool> 	_Accept;
		std::pair<std::string, bool> 	_SecFetchSite;
		std::pair<std::string, bool> 	_SecFetchMode;
		std::pair<std::string, bool> 	_SecFetchDest;
		std::pair<std::string, bool> 	_Referer;
		std::pair<std::string, bool> 	_AcceptEncoding;
		std::pair<std::string, bool> 	_dnt;
		std::pair<std::string, bool> 	_contentType;
		std::pair<std::string, bool>	_boundary;
		std::pair<std::string, bool>	_contentLength;
		std::pair<int, bool>			_intContentLength;
		std::pair<std::string, bool>	_cookie;
		//std::string _AcceptLanguage;
		//std::string _sec_ch_ua;
		//std::string _sec_ch_ua_mobile;
		//std::string _UserAgent;
		//std::string _sec_ch_ua_platform;
		//std::string _Host;
	
		Location						*_location;

	public:
		HttpRequest();
		HttpRequest(std::string client_request);
		HttpRequest(const HttpRequest & src);
		HttpRequest & operator=(const HttpRequest & rhs);
		~HttpRequest();
};

std::ostream &	operator<<( std::ostream & o, HttpRequest const & rhs);

std::string buildLocalPath(HttpRequest &request);

#endif
