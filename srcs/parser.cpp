#include "parser.hpp"

std::string read_file(char *config_file)
{
	int		fd;
	char	buff[BUFFER_SIZE];
	int		ret;
	std::string file_content;


	memset(buff,0, sizeof(buff));
	fd = open(config_file, O_RDONLY);
	if(fd < 0)
		throw exceptWebserv("Config Error : can't open the file");
	ret = read(fd, buff, BUFFER_SIZE);
	if(ret == -1)
		throw exceptWebserv("Config Error : something went wrong while reading the file");
	while(ret > 0)
	{
		file_content.append(buff);
		memset(buff,0, sizeof(buff));
		ret = read(fd, buff, BUFFER_SIZE);
	}
	close(fd);
	return (file_content);
}

void	remove_comment(std::string & content_file)
{
	std::string::iterator beg = content_file.begin();
	std::string::iterator end_of_comment;
	std::string::iterator end = content_file.end();

	while(beg != end)
	{
		if(*beg == '#')
		{
			end_of_comment = beg;
			while (*end_of_comment != '\n' && end_of_comment != end)
				end_of_comment++;
			content_file.erase(beg, end_of_comment);
		}
		beg++;
	}
}

void	remove_nl(std::string & content_file)
{
	content_file.erase(std::remove(content_file.begin(), content_file.end(), '\n'), content_file.end());
}

std::vector<std::string> split_vector(std::string str, std::string delimiter)
{
	std::vector<std::string>	splitted;
	std::string::iterator		beg = str.begin();
	std::string::iterator		end = str.end();
	std::string::iterator		stop = beg;

	while (beg != end)
	{
		stop = beg;
		size_t del = delimiter.find(*stop, 0);
		while(del == std::string::npos && stop != end)
		{
			stop++;
			del = delimiter.find(*stop, 0);
		}
		if(stop != beg)
		{
			std::string word(beg, stop);
			splitted.push_back(word);
		}
		if(del != std::string::npos && del > 1)
		{
			std::string word;
			word += delimiter[del];
			splitted.push_back(word);
			stop++;
		}
		if(stop != end)
			beg = stop + 1;
	}
	return(splitted);
}

void parser(char *config_file)
{
	std::string content_file = read_file(config_file);
	remove_comment(content_file);
	remove_nl(content_file);
	//std::cout << content_file << std::endl;

	std::string delimiter(" \t;{}");

	std::vector<std::string> splitted = split_vector(content_file, delimiter);

	//std::vector<std::string>::iterator beg = splitted.begin();

	for(std::vector<std::string>::iterator beg = splitted.begin(); beg != splitted.end(); beg++)
		std::cout << *beg << std::endl;
	
}