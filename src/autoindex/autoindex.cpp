#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <time.h>

#include <iostream>
#include <iomanip>

std::string setLink(std::string dname)
{
	std::ostringstream output;

	output << "<td><a href=\"" << dname << "\">" << dname << "</a></td>";
	return output.str();
}

std::string setSize(struct stat &buf)
{
	std::ostringstream output;

	output << "<td>" << buf.st_size << " Bytes" << "</td>";
	return output.str();
}

std::string setCreationTime(struct stat &buf)
{
	std::ostringstream output;
	struct tm dt;

	dt = *(gmtime(&buf.st_ctime));
	output << "<td>" << std::setfill('0') << std::setw(2) << dt.tm_mday << "/" << std::setw(2) << (dt.tm_mon + 1) << "/" << dt.tm_year + 1900 << " " << dt.tm_hour << ":" << std::setw(2) << dt.tm_min << ":" << std::setw(2) << dt.tm_sec << "</td>";
	return output.str();
}

std::string setModificationTime(struct stat &buf)
{
	std::ostringstream output;
	struct tm dt;

	dt = *(gmtime(&buf.st_mtime));
	output << "<td>" << std::setfill('0') << std::setw(2) << dt.tm_mday << "/" << std::setw(2) << (dt.tm_mon + 1) << "/" << dt.tm_year + 1900 << " " << dt.tm_hour << ":" << std::setw(2) << dt.tm_min << ":" << std::setw(2) << dt.tm_sec << "</td>";
	return output.str();
}

std::string 		createDirList(std::string path, std::string dname)
{
	std::string output;
	std::string info = path + dname;
	// lstat
	struct stat buf;

	if (lstat(info.c_str(), &buf) < 0)
		throw std::runtime_error("Error: lstat failed");

	output += "<tr>";
	output += setLink(dname);
	output += setSize(buf);
	output += setCreationTime(buf);
	output += setModificationTime(buf);
	output += "</tr>";

	return output;
}
std::string createFirstPart(std::string &path)
{
	std::ostringstream output;

	output << "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"stylesheet\" href=\"style.css\"></head>";
	output << "<style media=\"screen\">:root{background-color: #dddddd;font: message-box;}body{border: 1px solid #999999;border-radius: 10px;padding: 3em;min-width: 30em;max-width: 65em;margin: 4em auto;background-color: #fefefe;}h1{font-size: 160%;margin: 0 0 .6em;border-bottom: 1px solid #999999;font-weight: normal;}table{width: 90%;margin: 0 auto;}thead{font-size: 130%;}th{text-align: start;}td{white-space: nowrap;}a{text-decoration: none;}</style>";
	output << "<body><h1>Index of ";
	if (path.find_last_of("/") != std::string::npos)
		output << path.substr(path.find_last_of("/"));
	else
		output << path;
	output << "</h1><table><thead><tr><th>Name</th><th>Size</th><th>Creation Date</th><th>Last Modified</th></tr></thead><tbody>";

	return output.str();
}

std::string createAutoindexPage(std::string path)
{
	std::string page;
	// opendir
	DIR *dirp;
	// readdir
	struct dirent *dir;


	if (path.back() != '/')
		path += '/';

	if ((dirp = opendir(path.c_str())) == NULL)
	{
		std::cerr << "Error: " << strerror(errno) << '\n';
		return "";
	}

	std::string test = path.substr(0, path.size() - 1);
	try
	{
		page += createFirstPart(test);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << '\n';
		closedir(dirp);
		return "";
	}
	page += "<a href=\"..\">↵ Return to parent directory</a>";
	while ((dir = readdir(dirp)) != NULL)
		if (dir->d_name[0] != '.')
			page += createDirList(path, dir->d_name);
	page += "</tbody></table></body></html>";
	closedir(dirp);
	return page;
}
int main(int ac, char const **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./a.out [dir_to_display]" << '\n';
		return 0;
	}
	std::string page;
	page = createAutoindexPage(av[1]);
	if (!page.empty())
		std::cout << page << '\n';
	return 0;
}
