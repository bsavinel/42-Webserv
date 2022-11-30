/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpM_Post.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburat-d <nburat-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 14:43:54 by rpottier          #+#    #+#             */
/*   Updated: 2022/11/29 13:41:01 by nburat-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpM_Post.hpp"

void	HttpManager::methodPOST( void )
{
	std::string nbForFileName;

	if (_headerBuild == false)
	{
		_tmpFileName = getFileName();
		_tmp_upload.open(_tmpFileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
		if (_tmp_upload.fail())
		{
			std::cout << "_tmp_upload.open() failed" << std::endl;
			_errorCode = 500;
			return ;
		}
		_respond = "HTTP/1.1 204 No Content\n\n";
		_headerBuild = true;
	}
	if (_tmpEnd == false)
	{
		_tmp_upload.write(_request.getRequest().c_str(), _request.getRequest().size());
		if (_requestFullyReceive == true)
		{
			if (_request.getContentType().first.find("multipart/form-data") == 0)
			{
				if (parseMultiPart(_tmp_upload) == false)
				{
					canWrite();
					_tmpEnd = true;
					remove(_tmpFileName.c_str());
					return ;
				}
			}
			remove(_tmpFileName.c_str());
		}
	}
	if (_tmpEnd == true)
		_isEnd = true;
	else if (_requestFullyReceive == true) // Quand la requete est completement recue, on veut que _isEndsoit true au prochain tour de boucle
	{
		canWrite();
		_tmpEnd = true;
	}
	_request.getRequest().clear();
}

int HttpManager::parseMultiPart(std::fstream &fstream)
{
    std::string				BoundaryStartToFind = "--" + _request.getBoundary().first + "\r";
    std::string				BoundaryEndtoFind = "--" + _request.getBoundary().first + "--\r" ;
    std::string				str;
    std::string				boundaryHeader;
	bool					skipPart = false;

    fstream.seekg(0);
    while (fstream.eof() != true && _process.boundaryEnd == false)
    {
        if (_new_process == false)
        {
            _process = createProcess();
            _new_process = true;
        }
        if (_process.boundaryStart == false)
        {
            getline(fstream, str);
            if (str.compare(BoundaryStartToFind) == 0)
                _process.boundaryStart = true;
        }
        while (fstream.eof() != true && _process.boundaryStart == true && _process.header == false)
        {
            getline(fstream, str);
			std::string fileName;
            if (str.compare("\r") == 0) // si on trouve \r, on a fini de recuperer le header de la partie
            {
                _multipart_param = getParamBoundary(boundaryHeader);
                _process.header = true;
				fileName = getUploadFileName();
				if (fileName.empty())
				{
					skipPart = true;
					break;
				}
				if (fileExist(fileName))
				{
					std::cout << "File already exist." << std::endl;
					skipPart = true;
					break;
				}
				_uploaded.open(fileName.c_str(), std::fstream::app | std::fstream::in | std::fstream::out);
				if (_uploaded.fail())
				{
					std::cout << "Failed opening upload file." << std::endl;
					_errorCode = 500;
					return (false);
				}
			}
            else
            {
                boundaryHeader.append(str, 0, str.length());
                str.clear();
            }
        }
        if (_process.header == true)
        {
			std::string file;
            bool first = true;
            bool carriageReturn = false;
            int i = 0;
            while (fstream.eof() != true && str.compare(BoundaryEndtoFind) != 0 && str.compare(BoundaryStartToFind) != 0)
            {
                getline(fstream, str);
                if (str.compare(BoundaryEndtoFind) != 0 && str.compare(BoundaryStartToFind) != 0)
                {
                    if (!first && skipPart == false)
                        file += "\n"; 
                    if (str.size() >= 1 && carriageReturn == true)
                    {
						if (skipPart == false)
                        	file +=  '\r';
                        carriageReturn = false;
                    }
                    first = false;
                    if (str.size() >= 1 && (str[str.end() - str.begin()]) == '\r')
                    {
                        carriageReturn = true;
                        str.erase(str.size() - 1);
                    }
					if (str.size() >= 1 && skipPart == false)
						file +=  str;
                }
                i++;
            }
			file.erase(file.end() - 1);
			_uploaded << file;
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
				skipPart = false;
            }
            else
                _process.boundaryEnd = true;
        }
    }
	return (true);
}

std::string			HttpManager::getUploadFileName( void )
{
	std::string fileName;
	int status;

	if (_request.getLocation()->getUploadDirectory().empty())
	{
		status = mkdir("./uploads/", 0777);
		if ((status < 0) && (errno != EEXIST))
			return (fileName);
		fileName = "./uploads/" + _multipart_param.fileName.first;
	}
	else
	{
		status = mkdir(_request.getLocation()->getUploadDirectory().c_str(), 0777);
		if ((status < 0) && (errno != EEXIST))
			return (fileName);
		fileName = _request.getLocation()->getUploadDirectory() + _multipart_param.fileName.first;
	}
	return (fileName);
}

void printAscii(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		std::cout << (int)str[i] << " ";
	std::cout << std::endl;
}

void printMultiPartParam(t_multipart_param multipart_param)
{
	std::cout << "contentDisposition: " << multipart_param.contentDisposition.first << std::endl;
	std::cout << "fileName: " << multipart_param.fileName.first << std::endl;
	std::cout << "contentType: " << multipart_param.contentType.first << std::endl;
}

t_multipart_param	getParamBoundary(std::string boundaryHeader)
{
	t_multipart_param		multipart_param;
	std::string				toFind;
	std::string::size_type	startPos = 0;
	std::string::size_type	endPos = 0;
	std::string::size_type	subStrLen = 0;

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

bool fileExist (const std::string& name)
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
	}	while (fileExist(fileName));
	return fileName;
}

std::string getNbForFileName( void )
{
	std::string			nbForFileName;
	std::stringstream	ss;
	static int			nb_fd;

	ss << nb_fd;
	nbForFileName = ss.str();
	nb_fd++;
	return nbForFileName;
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
