/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpottier <rpottier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:44:50 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/28 14:44:51 by rpottier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTO_INDEX_HPP
# define AUTO_INDEX_HPP

#include "HttpManager.hpp"
#include "iostream"
#include <dirent.h>

std::string	autoIndex(HttpRequest &request, HttpManager &manager);
int	tryToGetFolder(std::string url);
int	isRootDirectory(std::string fileName);
std::string getParentFolderPath(std::string const & folderPath);
std::string constructPatchFromLocationBlock(Location const * location, std::string const & fileName);
std::string getAbsolutePathDirectory(std::string const & folderPath, std::string & fileName);
std::string insertHtmlHeader(std::string title);
std::string insertHtmlClosing(void);
std::string insertHtmlReference(std::string link, std::string userText);

#endif