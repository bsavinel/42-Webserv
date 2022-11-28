/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_Delete.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:42 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:43:43 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpManager.hpp"
#include <iostream>
#include <stdio.h>

void	HttpManager::methodDELETE()
{
	std::string	path_to_remove;
	int			exit_code;

	canWrite();
	if (_headerBuild == false)
	{
		path_to_remove = retrieveCorrespondingLocalPath();
		exit_code = remove(path_to_remove.c_str());
		if (exit_code == 0)
			_errorCode = 204;
		else
		{
			if (errno == ENOENT)
				_errorCode = 404;
			else if (errno == EINVAL || errno == ENAMETOOLONG || errno == ELOOP)
				_errorCode = 400;
			else
				_errorCode = 403;
		}
		_headerBuild = true;
	}
	else
		_isEnd = true;
}