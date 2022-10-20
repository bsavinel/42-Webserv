#include "HttpManager.hpp"
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int			g_nb_fd = 0;
std::string	getNbForFileName( void );
int			openUploadFile( void );



void	HttpManager::postMethod()
{
	std::string nbForFileName;
	std::string fileName;
//	int			tmp_upload_fd;

	std::cout << "_tmp_upload_fd = [" << _tmp_upload_fd << "]" << std::endl;  
	if (_tmp_upload_fd == -1)
	{
		std::cout << "TEMOIN" << std::endl;
		nbForFileName	= getNbForFileName();
		fileName		= "tmpUploadFile_" + nbForFileName;
		_tmp_upload_fd	= openUploadFile();
	}
	if (_tmp_upload_fd == -1)
	{
		perror("failed open: ");
		return ;
	}
	std::cout << "fileName = [" << fileName << "]" << std::endl;  
//	std::cout << "================="<<std::endl;
//	std::cout << _request.getRequest() << std::endl;
//	std::cout << "================="<<std::endl;

	_isEnd = true;
}

int openUploadFile( void )
{
	int			tmp_upload_fd;
	std::string nbForFileName	= getNbForFileName();
	std::string fileName		= "tmpUploadFile_" + nbForFileName;

//	std::cout << fileName << std::endl;
	tmp_upload_fd =  open(fileName.c_str(), O_CREAT | O_RDWR | O_EXCL);
//	std::cout << tmp_upload_fd << std::endl;
	return tmp_upload_fd;
}

std::string getNbForFileName( void )
{
	std::string nbForFileName;
	std::stringstream ss;

	ss << g_nb_fd;
	g_nb_fd++;
	nbForFileName = ss.str();
	return nbForFileName;
}