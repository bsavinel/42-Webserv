/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:47 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:48 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <netinet/in.h>
# include <sys/socket.h>
# include <string.h>
# include <errno.h>
# include <iostream>
# include <climits>
# include "exceptWebserv.hpp"
# include "Server.hpp"

# include <utility>
# include "Location.hpp"
# include "utils.hpp"


class Server
{
	private:
		int									_domain; // AF_INET, AF_INET6, AF_UNSPEC
		int									_service; // SOCK_STREAM, SOCK_DGRAM
		int									_protocol; // use 0 for "any"
		u_long								_interface; // needs to be set to INADDR_ANY
		int									_port;
		int									_backlog; // maximum number of queued clients
		struct sockaddr_in					_address;
		int									_socket;
		std::string							server_name;
		std::map<int, std::string>			error_map; // First error_code, Second path_to_html;
		int									client_max_body_size;
		std::map<std::string, Location*>	locations; //First = Path-of-location, Second = Object Location

	public:
		Server();
		Server(const Server & src);
		Server & operator=(const Server & rhs);
		~Server();

		
		void	launch();
		void	setConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
		bool	checkRacineLocationExist();


				// ----------------------
				// |		GETTERS		|
				// ----------------------

				const int&							getDomain() const;
				const int&							getService() const;
				const int&							getProtocol() const;
				u_long								getInterface() const;
				const int&							getPort() const;
				const int&							getBacklog() const;
				const int&							getSocket() const;
				struct sockaddr_in					getAddress() const;		
				const std::string&					getServerName() const;
				const std::map<int, std::string>	&getErrorMap() const;
				const int&							getClientMaxBodySize() const;
				std::map<std::string, Location*>	getLocationsMap() const;

				
				// ----------------------
				// |		PRINTS		|
				// ----------------------

				void	printConfig();

	private :
		void	add_location_block(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
		bool	is_path_stored_yet(std::string path);
		void	set_error_pages(std::vector<std::string>::iterator & it);
		void	set_client_max_body_size(std::vector<std::string>::iterator & it);



				// ----------------------
				// |	EXCEPTION		|
				// ----------------------
				class exceptionServer:  public std::exception
				{
					private:
						std::string _content;

					public :
						exceptionServer(const std::string content) throw();
						const char *what() const throw();
						~exceptionServer() throw();
				};

};

#endif