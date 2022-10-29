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


void printMultiPartParam(t_multipart_param multipart_param)
{
	std::cout << "contentDisposition: " << multipart_param.contentDisposition.first << std::endl;
	std::cout << "contentType: " << multipart_param.contentType.first << std::endl;
	std::cout << "fileName: " << multipart_param.fileName.first << std::endl;
}

t_multipart_param	getParamBoundary(std::string boundaryHeader)
{
	t_multipart_param multipart_param;
	std::string toFind;
	std::string::size_type startPos;
	std::string::size_type endPos;
	std::string::size_type subStrLen;

	// std::cout << "boundaryHeader/" << std::endl;
	// std::cout << boundaryHeader << std::endl;
	// std::cout << "/boundaryHeader" << std::endl;

	toFind = "Content-Disposition: ";
	
	if ((startPos = boundaryHeader.find(toFind.c_str(), 0)) != boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find(";", 0);
		subStrLen = endPos - startPos;
		multipart_param.contentDisposition.first = boundaryHeader.substr(startPos, subStrLen);
		multipart_param.contentDisposition.second = true;
	}

	toFind = "filename=";

	if ((subStrLen = boundaryHeader.find(toFind.c_str(), 0)) != boundaryHeader.npos)
	{
		startPos = startPos + toFind.length();
		endPos = boundaryHeader.find("\n", startPos);
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

void printAscii(std::string str)
{
	std::cout << "---------- ASCII -----------" << std::endl;
	for (size_t i = 0; i < str.length(); i++)
		std::cout << (int)str[i] << " ";
	std::cout << std::endl;

	std::cout << "---------- ASCII END -----------" << std::endl;
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

void HttpManager::parseMultiPart(std::fstream &fstream)
{
	fstream.seekg(std::fstream::beg);
	t_multipart_param	multipart_param;
	int					multipart_param_is_set = false;
	std::string			str;
	std::string			BoundaryStartToFind;
	std::string			BoundaryEndtoFind;
	std::string			boundaryHeader;
	std::string			boundaryBody;

	BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
	BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;

	std::cout << "Start multiparse" << std::endl;

	while (fstream.eof() != true)
	{
		getline(fstream, str);
		// on trouve un block boundary,

		if (str.compare(BoundaryStartToFind) == 0)
		{
			//  on doit dabord récupérer le header
			getline(fstream, str);
			while (str.compare("\r") != 0)
			{
//				std::cout << "str :" << str << std::endl;
//				printAscii(str);
				if ( str.size() && str[str.size()-1] == '\r' )
					str = str.substr( 0, str.size() - 1 );
				else
					str = str;
					
				boundaryHeader += str;
				str.clear();
				getline(fstream, str);
			}

			// std::cout << "boundaryHeader/" << std::endl;
			// std::cout << boundaryHeader << std::endl;
			// std::cout << "/boundaryHeader" << std::endl;
			multipart_param = getParamBoundary(boundaryHeader);

			// std::cout << "multipart_param/" << std::endl;
			// printMultiPartParam(multipart_param);
			// std::cout << "/multipart_param" << std::endl;
			
			while (str.compare(BoundaryEndtoFind) != 0)
			{
				boundaryBody += str;
				getline(fstream, str);
			}
		}
	}

	std::cout << "End multiparse" << std::endl;
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
		tmp_upload_fd =  open(fileName.c_str(), O_CREAT | O_RDWR | O_EXCL, 0777);

	}
	while ((tmp_upload_fd == -1) && errno == EEXIST);

	if (tmp_upload_fd == -1)
		perror("failed open: ");
	return tmp_upload_fd;
}
