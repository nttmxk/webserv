#ifndef CGI_HPP
# define CGI_HPP

#include <sstream>
#include <string>
#include <unistd.h>

class Cgi
{
private:
	/* data */
public:
	Cgi(/* args */);
	Cgi(const Cgi &cgi);
	~Cgi();

	void clear();
	void kill();

	int runCgi();
	void initCgi();
	int parseCgiResponse();
	int parseStatusHeader(std::stringstream &ss);
	size_t parseContentLength(std::stringstream &ss);

public:
	bool status;
	pid_t pid;
	int fd[2];
};

Cgi::Cgi(/* args */)
{
}

Cgi::~Cgi()
{
}


#endif