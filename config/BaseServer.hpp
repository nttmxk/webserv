#ifndef BASE_CONFIG_SEVER
# define BASE_CONFIG_SEVER
# include <iostream>
# include <string>
# include <fstream>
# include <algorithm>
# include <stdexcept>
# include <vector>
# include <map>
#include <stdio.h>
#include <sstream>
#include "Config_struct.hpp"
/*	Request class
	author	:kbaek
	date 	:2022.12.25
*/

class BaseServer
{
public:

    BaseServer(){
		BServer.serverName = "";
		BServer.host = "";
		BServer.port = "";
		BServer.maxRequestBodySize = -1;
	};
	BaseServer(const std::string& src) {};
    ~BaseServer() {};

	void setBServer(std::string fo, std::string str);
	void setBlcation(std::string fo,  Location location);
	void setBCgi(std::string fo, CgiConfig cgi);
	void setError(std::string str, std::vector<int> v);


	bool getRedirect();
	ServerInfo getBServer();
	std::map< std::string, CgiConfig > getBCgi();
	std::map< std::string, Location > getBLocation();

protected:
	int str_is_digit(const std::string &str);
	template <typename T>
	std::string printPair8(const T &iterator, bool nl);

protected:
    ServerInfo 							BServer;
    std::map< std::string, Location >	BLocation;
	std::map< std::string, CgiConfig >		BCgi;
	bool								redirect;
};

void BaseServer::setBServer(std::string fo, std::string str)
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
	{
		std::cout << "str = [" << str << "]" << std::endl;
		BServer.serverName = str;
	}
}

void BaseServer::setBlcation(std::string fo,  Location location)
{
	BLocation.insert(std::make_pair(fo, location));
}

void BaseServer::setError(std::string str, std::vector<int> v)
{
	BServer.errorPages.insert(std::make_pair(str, v));
}

void BaseServer::setBCgi(std::string fo, CgiConfig cgi)
{
	BCgi.insert(std::make_pair(fo, cgi));
}

ServerInfo BaseServer::getBServer()
{
	return this->BServer;
}

std::map< std::string, CgiConfig > BaseServer::getBCgi()
{
	return this->BCgi;
}

std::map< std::string, Location > BaseServer::getBLocation()
{
	return this->BLocation;
}
bool BaseServer::getRedirect()
{
	return this->redirect;
}

int str_is_digit(const std::string &s)
{
	std::cout << s << std::endl;
	if (s.find_first_not_of("0123456789") == std::string::npos)
		return 1;
	return 0;
}

#endif