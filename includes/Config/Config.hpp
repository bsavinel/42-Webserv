/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:42 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:43 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <iostream>
#include <list>

#include "utils.hpp"
#include "Server.hpp"

class Config
{
	private:
		std::list<Server*> servers;

	public:
		Config();
		Config(const Config & src);
		Config & operator=(const Config & rhs);
		~Config();

		void init(char *config_file);

				// ----------------------
				// |		GETTERS		|
				// ----------------------

				std::list<Server*> &	getServersList();


				// ----------------------
				// |		PRINTS		|
				// ----------------------

				void					print_all_conf();

	private :
				bool					checkLocBlock();
				Server* 				getServerToken(std::vector<std::string>::iterator & it, std::vector<std::string> & splitted);
				void 					look_for_and_initialise_server_block(std::vector<std::string>::iterator &beg, std::vector<std::string> &splitted);



};


#endif