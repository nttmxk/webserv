
#include "BaseServer.hpp"

void
BaseServer::setBServer(std::string fo, std::string str)
{
	if (fo == "h")
		BServer.host = str;
	else if (fo == "p")
	{
		int i;
		std::stringstream ssInt(str);
		ssInt >> i;
		BServer.port = i;
	}
	else if (fo == "s")
		BServer.serverName = str;
}

void
BaseServer::setBlcation(std::string fo,  Location location)
{
	BLocation.insert(std::make_pair(fo, location));
}

void
BaseServer::setError(std::string str, std::vector<int> v)
{
	BServer.errorPages.insert(std::make_pair(str, v));
}

void
BaseServer::setBCgi(std::string fo, CgiConfig cgi)
{
	BCgi.insert(std::make_pair(fo, cgi));
}

ServerInfo
BaseServer::getBServer()
{
	return this->BServer;
}

std::map< std::string, CgiConfig >
BaseServer::getBCgi()
{
	return this->BCgi;
}

std::map< std::string, Location >
BaseServer::getBLocation()
{
	return this->BLocation;
}

bool
BaseServer::getRedirect()
{
	return this->redirect;
}

int
BaseServer::str_is_digit(const std::string &s)
{
	if (s.find_first_not_of("0123456789") == std::string::npos)
		return 1;
	return 0;
}
