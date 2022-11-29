/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_redir.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:57 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 09:11:20 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"

bool HttpManager::manageRedirection()
{
	char buffer[LEN_TO_READ];
	int nb_char = LEN_TO_READ;

	if (_request.getLocation()->getRedirectionPath().empty())
		return false;
	if (_RedirectionStart == false)
	{
		_RedirectionStart = true;
		_respond.clear();
		if (!init_error_file(_request.getLocation()->getRedirectionPath(), _respond, _request.getLocation()->getReturnCode()))
			return false;
	}
	else
	{
		nb_char = read(_file_fd, buffer, LEN_TO_READ);
		if (nb_char > 0)
			_respond.insert(_respond.size(), &buffer[0], nb_char);
		if (nb_char < LEN_TO_READ)
		{
			close(_file_fd);
			_isEnd = true;
		}
		if (nb_char == -1)
		{
			_respond.clear();
			_respond = initDefaultErrorResponse();
			_isEnd = true;
		}
	}
	return true;
}