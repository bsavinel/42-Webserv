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

bool exists (const std::string& filename);
std::string	getNbForFileName( void );
int openUploadFile( void );

typedef struct s_multipart_param
{
	std::pair<std::string, bool>	contentDisposition;
	std::pair<std::string, bool>	fileName;
	std::pair<std::string, bool>	contentType;
}	t_multipart_param;

t_multipart_param	getParamBoundary(std::string boundaryHeader)
{
	t_multipart_param multipart_param;
	std::string toFind;
	std::string::size_type startPos;
	std::string::size_type endPos;
	std::string::size_type subStrLen;

	toFind = "Content-Disposition: ";
	
	if ((startPos = boundaryHeader.find(toFind.c_str(), 0)) == boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find(";", 0);
		subStrLen = endPos - startPos;
		multipart_param.contentDisposition.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.contentDisposition.second = true;
	}

	toFind = "filename=";

	if ((subStrLen = boundaryHeader.find(toFind.c_str(), 0)) == boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find("\n", startPos);
		subStrLen = endPos - startPos;
		multipart_param.fileName.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.fileName.second = true;
	}

	toFind = "Content-Type: ";
	
	if ((startPos = boundaryHeader.find(toFind.c_str(), 0)) == boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find(";", 0);
		subStrLen = endPos - startPos;
		multipart_param.contentType.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.contentType.second = true;
	}
	return multipart_param;
}

void HttpManager::parseMultiPart(std::fstream &fstream)
{
	t_multipart_param multipart_param;
	std::string boundary = this->_request.getBoundary().first;
	std::string str;
	std::string BoundaryStartToFind;
	std::string BoundaryEndtoFind;	
	std::string boundaryHeader;

	BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
	BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;

	fstream.seekg(std::fstream::beg);

	while (fstream.eof() == false)
	{
		getline(fstream, str);

		if (str.compare(BoundaryStartToFind) == 0)
		{
			std::cout << "Found BoundaryStartToFind" << std::endl;
			getline(fstream, str);
			while (str.compare(BoundaryEndtoFind) != 0)
			{
				boundaryHeader += str;
				getline(fstream, str);
			}
			std::cout << "Found BoundaryEndtoFind" << std::endl;
			std::cout << "boundaryHeader" << std::endl;
			std::cout << boundaryHeader << std::endl;
		}
	}
}

void	HttpManager::postMethod()
{
	std::string nbForFileName;

	if (_headerBuild == false)
	{
		_tmpFileName = getFileName();
		_tmp_upload.open(_tmpFileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
		if (_tmp_upload_o.fail())
			return ;
		_respond = "HTTP/1.1 204 No Content\n\n";
		_headerBuild = true;
	}
	if (_tmpEnd == false)
	{
		_tmp_upload << _request.getRequest().c_str();
	}
	if (_tmpEnd == true)
		_isEnd = true;
	else if (_requestFullyReceive == true) // Quand la requete est completement recue, on veut que _isEndsoit true au prochain tour de boucle
	{
		parseMultiPart(_tmp_upload);
		canWrite();
		_tmpEnd = true;
	}
	_request.getRequest().clear();
}

bool file_exist (const std::string& name)
{
	struct stat	buffer;   
	return (stat (name.c_str(), &buffer) == 0); 
}

std::string HttpManager::getFileName()
{
	std::string		nbForFileName;
	std::string		fileName;
	std::ofstream	stream;

	do
	{
		nbForFileName	= getNbForFileName();
		fileName		= DIR_PATH_TMP_UPLOAD_FILE + nbForFileName;
	}	while (file_exist(fileName));
	return fileName;
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

bool exists (const std::string& filename)
{
  struct stat buffer;   
  return (stat (filename.c_str(), &buffer) == 0); 
}


// void	HttpManager::postMethod()
// {
// 	std::string nbForFileName;
// 	std::string fileName;
// 	int ret = 0;

// 	if (_headerBuild == false)
// 	{
// 		if ((_tmp_upload_fd	= openUploadFile()) == -1)
// 			return ;
// 		_respond = "HTTP/1.1 204 No Content\n\n";
// 		_headerBuild = true;
// 	}
// 	if (_tmpEnd == false)
// 	{
// 		if ((ret = write(_tmp_upload_fd, _request.getRequest().c_str(), _request.getRequest().size())) == -1)
// 			std::cout << "PROBLEM WRITE POST" << std::endl;
// 	}
// 	if (_tmpEnd == true)
// 		_isEnd = true;
// 	else if (_requestFullyReceive == true) // Quand la requete est completement recue, on veut que _isEndsoit true au prochain tour de boucle
// 	{
// 		canWrite();
// 		_tmpEnd = true;
// 	}
// 	_request.getRequest().clear();
// }

// int openUploadFile()
// {
// 	std::string	fileName;
// 	std::string nbForFileName;
// 	int			tmp_upload_fd = -1;
// 	std::string dirPath("./bsavinel/site/tmpFilePost/");

// 	do
// 	{
// 		nbForFileName	= getNbForFileName();
// 		fileName		= dirPath + "tmpUploadFile_" + nbForFileName;
// 		tmp_upload_fd =  open(fileName.c_str(), O_CREAT | O_RDWR | O_EXCL, 0777);

// 	}
// 	while ((tmp_upload_fd == -1) && errno == EEXIST);

// 	if (tmp_upload_fd == -1)
// 		perror("failed open: ");
// 	return tmp_upload_fd;
// }
