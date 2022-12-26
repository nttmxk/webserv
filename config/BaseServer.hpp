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
	void setError(std::string str, std::vector<int> v);

	bool getRedirect();
	ServerInfo getBServer();
	std::map< std::string, Location > getBLocation();

	// void printStruct();
	
	template <typename T>
	std::string printPair8(const T &iterator, bool nl);

protected:
    ServerInfo 							BServer;
    std::map< std::string, Location >	BLocation;
	bool								redirect;
};

void BaseServer::setBServer(std::string fo, std::string str)
{
	if (fo == "h")
		BServer.host = str;
	else if (fo == "p")
		BServer.port = str;
	else if (fo == "s")
	{
		std::cout << "str = [" << str << "]" << std::endl;
		BServer.serverName = str;
	}
	// else
	// 	BServer.maxRequestBodySize = atoi(str);
}

// template<typename T>
void BaseServer::setBlcation(std::string fo,  Location location)
{
	BLocation.insert(std::make_pair(fo, location));
}

void BaseServer::setError(std::string str, std::vector<int> v)
{
	BServer.errorPages.insert(std::make_pair(str, v));
	
    //  for (auto& item : BServer.errorPages)
    //     std::cout << item.first << " is: " << item.second << std::endl;
	
}

ServerInfo BaseServer::getBServer()
{
	return this->BServer;
}
std::map< std::string, Location > BaseServer::getBLocation()
{
	return this->BLocation;
}
bool BaseServer::getRedirect()
{
	return this->redirect;
}

#endif