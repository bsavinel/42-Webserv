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

std::string ltrim_wsp(std::string str)
{
	size_t start = str.find_first_not_of(WHITESPACE);
	return((start == std::string::npos) ? "" : str.substr(start));
}

std::string rtrim_wsp(std::string str)
{
	size_t end = str.find_last_not_of(WHITESPACE);
	return((end == std::string::npos) ? "" : str.substr(0, end + 1));
}

void parser(char *config_file_path)
{
	//t_config	configuration;
	std::string	content_file;
	std::string	line;

	content_file = read_file(config_file_path);
	content_file = rtrim_wsp(ltrim_wsp(content_file));
	std::cout << "-----BEGIN----" << std::endl;
	std::cout << content_file << std::endl; 
	std::cout << "-----END----" << std::endl;
}