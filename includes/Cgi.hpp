/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:45:13 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:45:14 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

class Cgi;

# include <iostream>
# include <string>
# include <vector>
# include <ctime>
# include <stdlib.h>
# include <sys/wait.h>
# include "HttpRequest.hpp"
# include "utils.hpp"

class Cgi
{
	private :

		char **						_arg;
		std::string					_path_to_script;
		std::string					_exec;
		char**						_env;
		std::string					_request;
		std::string					_output;
		size_t						_start;
		pid_t						_pid;
		int							_pip1[2];
		int							_pip2[2];
		std::vector<std::string>	_cookies;

	public :
		Cgi();
		~Cgi();

		void				initialise_env(HttpRequest &request, const Server &server, std::string& path);
		int					feedOutput();
		void				set_path_cgi(std::string path);
		void				set_argv();
		bool				execute();
		void				manage_output();
		void				cutOutput(int len);
		void				store_cookies();
		void				free_argenv();


		// ----------------------
		// |		PRINTS		|
		// ----------------------

		void				printEnv() const;
		void				printArg() const;

		// ----------------------
		// |		GETTERS		|
		// ----------------------

		const std::string				&getOutput() const;
		const std::string				&getScriptPath() const;
		const std::vector<std::string>	&getCookies() const;

};


#endif