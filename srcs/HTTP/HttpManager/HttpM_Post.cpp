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

	if (_headerBuild == false)
	{
		_tmpFileName = getFileName();
		_tmp_upload.open(_tmpFileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
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
		std::cout << "_request/" << std::endl;
//		std::cout << _request.getRequest() << std::endl;
		printAscii(_request.getRequest().c_str());
		std::cout << "/_request" << std::endl;
		std::cout << "-1" << std::endl;
		_tmp_upload << _request.getRequest().c_str();
		
		if (count >= 1)
			parseMultiPart(_tmp_upload);
//			exit (0);
		count++;
	}
	if (_tmpEnd == true)
		_isEnd = true;
	else if (_requestFullyReceive == true) // Quand la requete est completement recue, on veut que _isEndsoit true au prochain tour de boucle
	{
		std::cout << "FINISH" << std::endl;
//		remove(_tmpFileName.c_str());
		canWrite();
		_tmpEnd = true;
	}
	_request.getRequest().clear();
}

void HttpManager::parseMultiPart(std::fstream &fstream)
{
	fstream.seekg(std::fstream::beg);
	std::string			BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
	std::string			BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;
	std::string			str;
	std::string			boundaryHeader;
	t_multipart_param	multipart_param;

	std::cout << "_new_process = " << _new_process << std::endl;

	if (_new_process == false)
	{
		_process = createProcess();
		_new_process = true;
	}

	std::cout << "_new_process = "<< _new_process << std::endl;
	std::cout << "_process.boundaryStart = "<< _process.boundaryStart << std::endl;
	if (_process.boundaryStart == false)
	{
		std::cout << "0" << std::endl;
		getline(fstream, str);
		// if ( str.size() && str[str.size()-1] == '\r' )
		// 	str = str.substr( 0, str.size() - 1 );
		// else
		// 	str = str;
		std::cout << "HEEEEEERRRRRREEE : " << str << std::endl;
//		usleep(100000);
		// exit (0);
		// on trouve un block boundary,
		if (str.compare(BoundaryStartToFind) == 0)
		{
			_process.boundaryStart = true;

			std::cout << "_process.boundaryStart == true" << std::endl;
	//		return ;
		}
	}
	while (fstream.eof() != true && _process.boundaryStart == true && _process.header == false)
	{
		std::cout << "1" << std::endl;
		getline(fstream, str);
		if (str.compare("\r") == 0)
		{

			std::cout << "1.1" << std::endl;
			multipart_param = getParamBoundary(boundaryHeader);
			_process.header = true;
			std::cout << "multipart_param.fileName.first:" << multipart_param.fileName.first << std::endl;
			
 			std::string fileName = _request.getLocation()->getUploadDirectory() + multipart_param.fileName.first;

			std::cout << "fileName" << fileName << std::endl;
			_uploaded.open(fileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
			if (_uploaded.fail())
				std::cout << "failed open _uploadFile" << std::endl;
			std::cout << "multipart_param.fileName.first:" << multipart_param.fileName.first << std::endl;
		}
		else
		{
			std::cout << "1.2" << std::endl;
//			boundaryHeader += str;
			boundaryHeader.append(str, 0, str.length());
			str.clear();
		}
	}
	if (_process.header == true)
	{
		while (fstream.eof() != true && str.compare(BoundaryEndtoFind) != 0)
		{
			getline(fstream, str);
			// if ( str.size() && str[str.size()-1] == '\r' )
			// 	str = str.substr( 0, str.size() - 1 );
			// else
			// 	str = str;
			if (str.compare(BoundaryEndtoFind) != 0)
				_uploaded << str.c_str();
			
			std::cout << "2" << std::endl;
		}
	}
	if (str.compare(BoundaryEndtoFind) == 0)
	{
		_process.boundaryEnd = true;
		_uploaded.close();
		_new_process = false;
	}
}


// void HttpManager::parseMultiPart(std::fstream &fstream)
// {
// 	fstream.seekg(std::fstream::beg);

// 	t_multipart_param	multipart_param;
// 	t_process			process = createProcess();
// 	int					multipart_param_is_set = false;
// 	std::string			str;
// 	std::string			BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
// 	std::string			BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;
// 	std::string			boundaryHeader;
// 	std::string			boundaryBody;

// 	std::cout << "Start multiparse" << std::endl;

// 	while (fstream.eof() != true)
// 	{
// 		getline(fstream, str);
// 		// on trouve un block boundary,

// 		if (str.compare(BoundaryStartToFind) == 0)
// 		{
// 			//  on doit dabord récupérer le header
// 			getline(fstream, str);
// 			while (str.compare("\r") != 0)
// 			{
// //				std::cout << "str :" << str << std::endl;
// //				printAscii(str);
// 				if ( str.size() && str[str.size()-1] == '\r' )
// 					str = str.substr( 0, str.size() - 1 );
// 				else
// 					str = str;
					
// 				boundaryHeader += str;
// 				str.clear();
// 				getline(fstream, str);
// 			}

// 			// std::cout << "boundaryHeader/" << std::endl;
// 			// std::cout << boundaryHeader << std::endl;
// 			// std::cout << "/boundaryHeader" << std::endl;
// 			multipart_param = getParamBoundary(boundaryHeader);

// 			std::cout << "multipart_param/" << std::endl;
// 			printMultiPartParam(multipart_param);
// 			std::cout << "/multipart_param" << std::endl;
// 			std::fstream finalFile;
// 			std::string fileName = _request.getLocation()->getUploadDirectory() + multipart_param.fileName.first;
// 			std::cout << "filename: " << fileName << std::endl;
// 			finalFile.open(fileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
			
// 			while (str.compare(BoundaryEndtoFind) != 0)
// 			{
// 				std::cout << "TEMOIN" << std::endl;
// 				finalFile << str.c_str();
// //				boundaryBody += str;
// 				getline(fstream, str);
// 			}
// 		}
// 	}

// 	std::cout << "End multiparse" << std::endl;
// }



void printAscii(std::string str)
{
	std::cout << "---------- ASCII -----------" << std::endl;
	for (size_t i = 0; i < str.length(); i++)
		std::cout << (int)str[i] << " ";
	std::cout << std::endl;

	std::cout << "---------- ASCII END -----------" << std::endl;
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

	std::cout << "boundaryHeader/" << std::endl;
	// std::cout << boundaryHeader << std::endl;
	printAscii(boundaryHeader);
	std::cout << "/boundaryHeader" << std::endl;

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
