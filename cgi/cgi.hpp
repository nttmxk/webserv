#ifndef CGI_HPP
# define CGI_HPP
# include <cstdlib>
# include <iostream>
# include <unistd.h>
# include <fcntl.h>
# include <libproc.h>
# include <unistd.h>
# include <libgen.h>
# include "../parseRequest/Request.hpp"

class Cgi {
public:

	Cgi();
	~Cgi();
	Cgi(const Cgi &orig);
	Cgi& operator=(const Cgi &orig);

private:


};



#endif