#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <stdlib.h>

int main(void)
{
	DIR	*dir = opendir(".");
	struct dirent *dirent;

	if (dir == NULL)
		std::cout << "Error: enable to open directory." << std::endl;
	while ((dirent = readdir(dir)) != NULL)
		if (dirent->d_type == DT_DIR)
			std::cout << dirent->d_name << std::endl;
	closedir (dir);
	return (0);
}