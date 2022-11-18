#include "HttpManager.hpp"
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int			g_nb_fd = 0;
std::string	getNbForFileName( void );
int openUploadFile( std::string fileName );


void	HttpManager::methodPOST()
{
	std::string nbForFileName;
	std::string fileName;
//	int			tmp_upload_fd;

//	std::cout << "_tmp_upload_fd = [" << _tmp_upload_fd << "]" << std::endl;  
	if (_tmp_upload_fd == -1)
	{
		std::cout << "OPEN" << std::endl;
		nbForFileName	= getNbForFileName();
		fileName		= "tmpUploadFile_" + nbForFileName;
		_tmp_upload_fd	= openUploadFile(fileName);
	}
	if (_tmp_upload_fd == -1)
	{
		perror("failed open: ");
//		return ;
	}

//	unlink(fileName.c_str());
//	std::cout << "fileName = [" << fileName << "]" << std::endl;  
	// std::cout << "================="<<std::endl;
	// std::cout << _request.getRequest() << std::endl;
	// std::cout << "================="<<std::endl;
	// _request.getRequest().clear();
	// // if (_request.getRequest().empty() == true)
	// // 	std::cout << "BONJOUR" << std::endl;
	// _isEnd = true;
	// _respond =  buildHeader(_respond.size(), 200);
}

int openUploadFile( std::string fileName )
{
	int			tmp_upload_fd;
///	std::string nbForFileName	= getNbForFileName();
//	std::string fileName		= "tmpUploadFile_" + nbForFileName;

	tmp_upload_fd =  open(fileName.c_str(), O_CREAT | O_RDWR | O_EXCL);

//	std::cout << fileName << std::endl;
//	std::cout << tmp_upload_fd << std::endl;

	return tmp_upload_fd;
}

std::string getNbForFileName( void )
{
	std::string nbForFileName;
	std::stringstream ss;

	ss << g_nb_fd;
	nbForFileName = ss.str();
	g_nb_fd++;
	return nbForFileName;
}