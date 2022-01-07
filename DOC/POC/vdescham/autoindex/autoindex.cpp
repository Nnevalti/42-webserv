#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <set>

#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <time.h>

void 		display_lstat(struct stat buf)
{
	struct tm dt;

	std::cout << "File Access: ";
	if (buf.st_mode & R_OK)
		std::cout << "read ";
	if (buf.st_mode & W_OK)
		std::cout << "write ";
	if (buf.st_mode & X_OK)
		std::cout << "execute";
	std::cout << '\n';

	std::cout << "File Size: " << buf.st_size << " bytes" << '\n';

	dt = *(gmtime(&buf.st_ctime));
	std::cout << "Created on: " << dt.tm_mday << "/" << dt.tm_mon + 1 << "/" << dt.tm_year + 1900 << " " << dt.tm_hour << ":" << dt.tm_min << ":" << dt.tm_sec << '\n';

	dt = *(gmtime(&buf.st_mtime));
	std::cout << "Modified on: " << dt.tm_mday << "/" << dt.tm_mon + 1 << "/" << dt.tm_year + 1900 << " " << dt.tm_hour << ":" << dt.tm_min << ":" << dt.tm_sec << '\n';

	std::cout << '\n';

}

int main(int ac, char **av)
{
	// opendir
	DIR *dirp;
	std::string path;
	// readdir
	struct dirent *dir;
	// lstat
	struct stat buf;
	std::string info;
	// display
	std::set<std::string> dir_content;

	if (ac != 2)
	{
		std::cerr << "Usage: ./a.out [dir_to_display]" << '\n';
		return 0;
	}

	path = av[1];
	// We try to open the directory
	dirp = opendir(path.c_str());
	if (dirp == NULL) // An error occured during the opening
	{
		std::cerr << "Error: " << strerror(errno) << '\n';
		return 0;
	}

	while ((dir = readdir(dirp)) != NULL) // reading the directory content
	{
		info = path + dir->d_name;
		std::cout << dir->d_name << ":\n";
		if (lstat(info.c_str(), &buf) < 0)
			std::cerr << "Error: lstat() " << strerror(errno) << '\n';
		display_lstat(buf);
		// if (dir->d_name[0] != '.') // This line is there if you don't want to display hidden files
		dir_content.insert(dir->d_name); // Store the information for later (in a set to have it sorted and for easy access)
	}

	// for (std::set<std::string>::iterator it = dir_content.begin(); it != dir_content.end(); it++)
	// {
	// 	std::cout << *it << " ";
	// }
	// std::cout << '\n';
	closedir(dirp); // close the directory to avoid leaks
	return 0;
}
