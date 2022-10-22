#include "HttpManager.hpp"
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int			g_nb_fd = 0;
std::string	getNbForFileName( void );
int openUploadFile(  );


void	HttpManager::postMethod()
{
	std::string nbForFileName;
	std::string fileName;
	int ret = 0;

	if (_headerBuild == false)
	{
		if ((_tmp_upload_fd	= openUploadFile()) == -1)
			return ;
//		_respond =  buildHeader(_respond.size(), 200);
		_respond = "HTTP/1.1 204 No Content\n\n";
		_headerBuild = true;
	}
	if (_tmpEnd == false)
	{
		if ((ret = write(_tmp_upload_fd, _request.getRequest().c_str(), _request.getRequest().size())) == -1)
			std::cout << "PROBLEM WRITE POST" << std::endl;
		
		// else if (ret < LEN_TO_READ)
		// 	canWrite();
	}
	if (_tmpEnd == true)
	{
	//	_request.getRequest().clear();
		_isEnd = true;
		return ;
	}
	if (_requestFullyReceive == true)
	{
		canWrite();
		_tmpEnd = true;
	}
	_request.getRequest().clear();
		//Is end = true quand la reponse est envoye
}

int openUploadFile()
{
	std::string	fileName;
	std::string nbForFileName;
	int			tmp_upload_fd = -1;
///	std::string nbForFileName	= getNbForFileName();
//	std::string fileName		= "tmpUploadFile_" + nbForFileName;

	do
	{
		nbForFileName	= getNbForFileName();
		fileName		= "tmpUploadFile_" + nbForFileName;
		tmp_upload_fd =  open(fileName.c_str(), O_CREAT | O_RDWR | O_EXCL, 0777);

	}
	while ((tmp_upload_fd == -1) && errno == EEXIST);

	if (tmp_upload_fd == -1)
		perror("failed open: ");
	return tmp_upload_fd;
}

std::string getNbForFileName( void )
{
	std::string nbForFileName;
	std::stringstream ss;
	static int nb_fd;
	ss << nb_fd;
	nbForFileName = ss.str();
	nb_fd++;
	return nbForFileName;
}