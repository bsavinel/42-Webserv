/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:44 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:45 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_BLOCK_HPP
#define LOCATION_BLOCK_HPP

# include <stdlib.h>
# include <ctype.h>

# include <string>
# include <iostream>
# include <vector>
# include <map>
# include <exception>

# include "utils.hpp"

class Location
{
	private:
		std::vector<std::string>	allowed_methods;
		int							return_code;
		std::string					redirection_path;
		std::string					root_path;
		std::string					index_path;
		bool						autoindex;
		std::string					cgi_file_extension;
		std::string					cgi_path_to_script;
		std::string					upload_dir;
		std::string					locate;

	public :
		Location();
		Location(const Location & src);
		Location & operator=(const Location & rhs);
		~Location();

		void	setConfig(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted, std::string &loc);

				// ----------------------
				// |		GETTERS		|
				// ----------------------

				const std::vector<std::string>&		getAllowedMethods() const;
				const int&							getReturnCode() const;
				const std::string&					getRedirectionPath() const;
				const std::string&					getRootPath() const;
				const std::string&					getIndexPath() const;
				const bool&							getAutoPath() const;
				const std::string&					getCgiFileExtension() const;
				const std::string&					getCgiPathToScript() const;
				const std::string&					getUploadDirectory() const;
				const std::string&					getLocate() const;

				// ----------------------
				// |		PRINTS		|
				// ----------------------

				void	printConfig();

				// ----------------------
				// |	EXCEPTION		|
				// ----------------------

	private :
		void	set_allowed_methods(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
		void	set_redirection(std::vector<std::string>::iterator & it);
		void	set_root_path(std::vector<std::string>::iterator & it);
		void	set_autoindex(std::vector<std::string>::iterator & it);
		void	set_cgi_pass(std::vector<std::string>::iterator & it);
		void	set_upload_store(std::vector<std::string>::iterator & it);
		void	check_param(std::vector<std::string>::iterator & it);
		void	check_both_params(std::vector<std::string>::iterator & it);




	class exceptionLocation :  public std::exception
	{
		private:
			std::string _content;
		public :
			exceptionLocation(const std::string content) throw();
			const char *what() const throw();
			~exceptionLocation() throw();
	};
};

#endif