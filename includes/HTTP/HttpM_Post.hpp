#ifndef HTTPM_POST_HPP
#define HTTPM_POST_HPP

#include "HttpManager.hpp"
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>

t_multipart_param	getParamBoundary(std::string boundaryHeader);
t_process			createProcess( void );
bool				fileExist (const std::string& name);
std::string			getNbForFileName( void );
int					openUploadFile();
void				printAscii(std::string str);
void				printMultiPartParam(t_multipart_param multipart_param);
std::string			getFileName( void );

#endif