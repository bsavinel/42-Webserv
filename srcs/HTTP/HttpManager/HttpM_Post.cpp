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

// probleme quand LEN_TO_READ est trop petit pour lire toute la requete
// la len indique dans le header de la requete et la len du BODY de la requete


void				printAscii(std::string str);
void				printMultiPartParam(t_multipart_param multipart_param);
t_multipart_param	getParamBoundary(std::string boundaryHeader);
t_process			createProcess( void );
// void				HttpManager::parseMultiPart(std::fstream &fstream);
// void				HttpManager::postMethod();
bool				file_exist (const std::string& name);
// std::string		HttpManager::getFileName();
std::string			getNbForFileName( void );
bool				exists (const std::string& filename);
int					openUploadFile();
std::fstream &		safegetline( std::fstream & fstream, std::string & line );
static int count;

void	HttpManager::postMethod()
{
	std::string nbForFileName;
//	bool		requestTreated = false;
	if (_headerBuild == false)
	{
		_tmpFileName = getFileName();
		_tmp_upload.open(_tmpFileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
		if (_tmp_upload_o.fail())
		{
			std::cout << "_tmp_upload.open() failed" << std::endl;
			exit (1) ;
		}
		_respond = "HTTP/1.1 204 No Content\n\n";
		_headerBuild = true;
	}
	if (_tmpEnd == false)
	{
// 		std::cout << "_requestinPOSTmethod/" << std::endl;
// 		std::cout << _request.getRequest() << std::endl;
// //		printAscii(_request.getRequest().c_str());
// 		std::cout << "/_request" << std::endl;
// 		std::cout << "-1" << std::endl;
		// if (_tmp_upload.is_open())
		// 	std::cout << "OPEN" << std::endl;
		// else
		// 	std::cout << "CLOSE" << std::endl;
		_tmp_upload.clear();
		
		if (count >= 1 )
		{
			_tmp_upload.write(_request.getRequest().c_str(), _request.getRequest().size());
//			parseMultiPart(_tmp_upload);
//			remove(_tmpFileName.c_str());
		}
		if (_requestFullyReceive == true)
		{
			parseMultiPart(_tmp_upload);
//			requestTreated = true;
		}
		count++;
	}
	if (_tmpEnd == true)
		_isEnd = true;
	else if (_requestFullyReceive == true) // Quand la requete est completement recue, on veut que _isEndsoit true au prochain tour de boucle
	{
//		std::cout << "FINISH" << std::endl;
//		remove(_tmpFileName.c_str());
		canWrite();
		_tmpEnd = true;
	}
	_request.getRequest().clear();
}
/*
void HttpManager::parseMultiPart(std::fstream &fstream)
{
	std::string			BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
	std::string			BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;
	std::string			str;
	std::string			boundaryHeader;
	int					uploadReady = false;
	int					boundaryHeaderReady = false;
	while (getline(fstream, str))
	{
		if(str.compare(BoundaryStartToFind) == 0)
			uploadReady = true;
		if (boundaryHeaderReady == true && str.compare("\r") == 0) // si on trouve \r, on a fini de recuperer le header de la partie
		{
			_multipart_param = getParamBoundary(boundaryHeader);
			_process.header = true;
			std::string fileName = _request.getLocation()->getUploadDirectory() + _multipart_param.fileName.first;
			_uploaded.open(fileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
			if (_uploaded.fail())
				std::cout << "failed open _uploadFile" << std::endl;
			uploadReady = false;
		}
		else if (uploadReady == true)
		{
			boundaryHeader.append(str, 0, str.length());
			str.clear();
		}
	}
	
}
*/
void HttpManager::parseMultiPart(std::fstream &fstream)
{
//	fstream.seekg(std::fstream::beg);
	fstream.seekg(_lenOfRequestAlreadyRead);
	std::cout << "std::fstream::beg = " << std::fstream::beg << std::endl;
	std::string			BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
	std::string			BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;
	std::string			str;
	std::string			boundaryHeader;
//	t_multipart_param	multipart_param;

	while (fstream.eof() != true && _process.boundaryEnd == false)
	{
//		std::cout << "_new_process = " << _new_process << std::endl;

		if (_new_process == false)
		{
//		std::cout << "_new_process = "<< _new_process << std::endl;
			_process = createProcess();
			_new_process = true;
		}

//		std::cout << "_process.boundaryStart = "<< _process.boundaryStart << std::endl;
		if (_process.boundaryStart == false)
		{
//			std::cout << "Searching BoundaryStartToFind..." << std::endl;
			getline(fstream, str);
			_lenOfRequestAlreadyRead += str.length() + 1;
			// on trouve un block boundary,
			if (str.compare(BoundaryStartToFind) == 0)
			{
				_process.boundaryStart = true;
//				std::cout << "BoundaryStartToFind is found!" << std::endl;
			}
		}
		// on veut ici recuperer ce qui est lu, jusqu 'a trouver un BoundaryEndtoFind ou une ligne ne se terminant pas \n
		while (fstream.eof() != true && _process.boundaryStart == true && _process.header == false)
		{
//			std::cout << "1: " << str << std::endl;
			getline(fstream, str);
			_lenOfRequestAlreadyRead += str.length() + 1;
			// std::cout <<  "STR=\n" << str << "\n=STR" << std::endl;
			// printAscii(str);
			if (str.compare("\r") == 0) // si on trouve \r, on a fini de recuperer le header de la partie
			{
				std::cout << "1.1" << std::endl;
				_multipart_param = getParamBoundary(boundaryHeader);
				_process.header = true;
				
				std::string fileName = _request.getLocation()->getUploadDirectory() + _multipart_param.fileName.first;

				_uploaded.open(fileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
				if (_uploaded.fail())
					std::cout << "failed open _uploadFile" << std::endl;
//				std::cout << "multipart_param.fileName.first:" << multipart_param.fileName.first << std::endl;
			}
			else
			{
//				std::cout << "1.2" << str << std::endl;
	//			boundaryHeader += str;
				boundaryHeader.append(str, 0, str.length());
				str.clear();
			}
		}
		if (_process.header == true)
		{
			while (fstream.eof() != true && str.compare(BoundaryEndtoFind) != 0 && str.compare(BoundaryStartToFind) != 0)
			{
				getline(fstream, str);


				_lenOfRequestAlreadyRead += str.length() + 1;
				if (str.compare(BoundaryEndtoFind) != 0 && str.compare(BoundaryStartToFind) != 0)
				{
					std::cout <<  "STR=\n" << str << "\n=STR" << std::endl;
				printAscii(str);
			//		int i = str.length() -1;
//					std::cout << *(str.end() - 1) << std::endl;

					_uploaded << str.c_str();
					_uploaded << '\n'; 
				}
			}
			std::cout << "2" << std::endl;
		}
		if (str.compare(BoundaryEndtoFind) == 0 || str.compare(BoundaryStartToFind) == 0)
		{
			_uploaded.close();
			_new_process = false;
			if (str.compare(BoundaryStartToFind) == 0)
			{
				_process = createProcess();
				_new_process = true;
				_process.boundaryStart = true;
				boundaryHeader.clear();
//				getline(fstream, str);
//				std::cout << "BoundaryStartToFind is found!" << std::endl;
//				parseMultiPart(fstream);
			}
			else
			{
				_process.boundaryEnd = true;
//				std::cout << "BoundaryEndToFind is found!" << std::endl;
			}
		}
	}
}

void printAscii(std::string str)
{
//	std::cout << "---------- ASCII -----------" << std::endl;
	for (size_t i = 0; i < str.length(); i++)
		std::cout << (int)str[i] << " ";
	std::cout << std::endl;

//	std::cout << "---------- ASCII END -----------" << std::endl;
}

void printMultiPartParam(t_multipart_param multipart_param)
{
	std::cout << "contentDisposition: " << multipart_param.contentDisposition.first << std::endl;
	std::cout << "fileName: " << multipart_param.fileName.first << std::endl;
	std::cout << "contentType: " << multipart_param.contentType.first << std::endl;
}

t_multipart_param	getParamBoundary(std::string boundaryHeader)
{
	t_multipart_param multipart_param;
	std::string toFind;
	std::string::size_type startPos = 0;
	std::string::size_type endPos = 0;
	std::string::size_type subStrLen = 0;

//	std::cout << "boundaryHeader/" << std::endl;
//	std::cout << boundaryHeader << std::endl;
//	printAscii(boundaryHeader);
//	std::cout << "/boundaryHeader" << std::endl;

	toFind = "Content-Disposition: ";
	
	if ((startPos = boundaryHeader.find(toFind.c_str(), 0)) != boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find(";", 0);
		subStrLen = endPos - startPos;
		multipart_param.contentDisposition.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.contentDisposition.second = true;
	}

	toFind = "filename=\"";

	if ((startPos = boundaryHeader.find(toFind.c_str(), 0)) != boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find("\"", startPos);
		subStrLen = endPos - startPos;
		multipart_param.fileName.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.fileName.second = true;
	}

	toFind = "Content-Type: ";
	
	if ((startPos = boundaryHeader.find(toFind.c_str(), 0)) != boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find(";", 0);
		subStrLen = endPos - startPos;
		multipart_param.contentType.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.contentType.second = true;
	}

	// std::cout << "multipart_param/" << std::endl;
	// printMultiPartParam(multipart_param);
	// std::cout << "/multipart_param" << std::endl;

	return multipart_param;
}

t_process	createProcess( void )
{
	t_process process;

	process.boundaryStart = false;
	process.header = false;
	process.body = false;
	process.boundaryEnd = false;

	return process;
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

int openUploadFile()
{
	std::string	fileName;
	std::string nbForFileName;
	int			tmp_upload_fd = -1;
	std::string dirPath("./bsavinel/site/tmpFilePost/");

	do
	{
		nbForFileName	= getNbForFileName();
		fileName		= dirPath + "tmpUploadFile_" + nbForFileName;
		tmp_upload_fd	=  open(fileName.c_str(), O_CREAT | O_RDWR | O_EXCL, 0777);
	}
	while ((tmp_upload_fd == -1) && errno == EEXIST);

	if (tmp_upload_fd == -1)
		perror("failed open: ");
	return tmp_upload_fd;
}

std::fstream & safegetline( std::fstream & fstream, std::string & line )
{
    std::string myline;
    if ( getline( fstream, myline ) ) {
       if ( myline.size() && myline[myline.size()-1] == '\r' ) {
           line = myline.substr( 0, myline.size() - 1 );
       }
       else {
           line = myline;
       }
    }
    return fstream;
}
