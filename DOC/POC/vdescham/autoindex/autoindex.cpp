#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <set>

#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <time.h>

// This POC can also be called "ls" lol

void 		display_lstat(std::string info)
{
	// lstat
	struct stat buf;

	if (lstat(info.c_str(), &buf) < 0)
		std::cerr << "Error: lstat() " << strerror(errno) << '\n';

	std::cout << "File type: ";
	switch (buf.st_mode & S_IFMT)
	{
		case S_IFBLK: std::cout << "block device" << '\n';
			break;
		case S_IFCHR: std::cout << "character device" << '\n';
			break;
		case S_IFDIR: std::cout << "directory" << '\n';
			break;
		case S_IFIFO: std::cout << "FIFO/pipe" << '\n';
			break;
		case S_IFLNK: std::cout << "symlink" << '\n';
			break;
		case S_IFREG: std::cout << "regular file" << '\n';
			break;
		case S_IFSOCK:std::cout << "socket" << '\n';
			break;
		default:      std::cout << "unknown?" << '\n';
			break;
	}

	std::cout << "File Access: ";

	// Some magic code I found online :D
	// Kidding I do understand the logic, I just couldn't implement that in a such pretty way
	const char chars[] = "rwxrwxrwx";
	for (size_t i = 0; i < 9; i++)
		std::cout << ((buf.st_mode & (1 << (8-i))) ? chars[i] : '-');
	std::cout << '\n';

	std::cout << "File Size: " << buf.st_size << " bytes" << '\n';

	struct tm dt;

	// std::string are so useful, I mean printf was cool but... yeah
	dt = *(gmtime(&buf.st_ctime));
	std::cout << "Created on: " << std::setfill('0') << std::setw(2) << dt.tm_mday << "/" << std::setw(2) << (dt.tm_mon + 1) << "/" << dt.tm_year + 1900 << " " << dt.tm_hour << ":" << dt.tm_min << ":" << dt.tm_sec << '\n';

	dt = *(gmtime(&buf.st_mtime));
	std::cout << "Modified on: " << std::setfill('0') << std::setw(2) << dt.tm_mday << "/" << std::setw(2) << (dt.tm_mon + 1) << "/" << dt.tm_year + 1900 << " " << dt.tm_hour << ":" << dt.tm_min << ":" << dt.tm_sec << '\n';
	std::cout << '\n';
}

int main(int ac, char **av)
{
	// opendir
	DIR *dirp;
	std::string path;
	// readdir
	struct dirent *dir;

	if (ac != 2)
	{
		std::cerr << "Usage: ./a.out [dir_to_display]" << '\n';
		return 0;
	}

	path = av[1];
	if (path.back() != '/')
		path += '/';
	// We try to open the directory
	dirp = opendir(path.c_str());
	if (dirp == NULL) // An error occured during the opening
	{
		std::cerr << "Error: " << strerror(errno) << '\n';
		// perror("Error"); // It's actually the same thing as above Wow
		return 0;
	}

	while ((dir = readdir(dirp)) != NULL) // reading the directory content
	{
		if (dir->d_name[0] != '.') // This line is there if you don't want to display hidden files
		{
			std::cout << dir->d_name << ":\n";
			display_lstat(path + dir->d_name);
		}
	}

	closedir(dirp); // close the directory to avoid leaks
	return 0;
}
