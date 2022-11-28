/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:45:00 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:45:01 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
# define EVENT_HPP

#include "Epoll.hpp"
#include "HttpManager.hpp"

void	clientEvent(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager);
void	serverEvent(Epoll &epoll, std::map<t_socket, HttpManager> &stockManager);

#endif