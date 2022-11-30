/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:45:21 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 13:51:32 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

class Socket;
#include "define.hpp"
#include "Epoll.hpp"
#include "HttpRequest.hpp"
#include "HttpManager.hpp"
#include <string>

class Socket
{
	public:
		Socket();
		Socket(const Socket &rhs);
		~Socket();
		Socket &operator=(const Socket &rhs);
		void	setRequest(std::string req);
		const HttpRequest	&getRequest(void) const;
		void	readSocket();

	private:
//		HttpManager *_manager;
		bool _init;

		HttpRequest	_request;
		t_socket	_fd;
		char *_str;
};

#endif