#ifndef BASE_CONFIG_SEVER
# define BASE_CONFIG_SEVER
# include <iostream>
# include <string>
# include <fstream>
# include <algorithm>
# include <stdexcept>
# include <vector>
# include <map>
# include <stdio.h>
# include <sstream>
# include "Config_struct.hpp"

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
		BServer.port = -1;
		BServer.maxRequestBodySize = -1;
	};
	// BaseServer(const std::string& src) {};
    ~BaseServer() {};

	void setBServer(std::string fo, std::string str);
	void setBlcation(std::string fo,  Location location);
	void setBCgi(std::string fo, CgiConfig cgi);
	void setError(std::string str, std::vector<int> v);


	bool getRedirect();
	ServerInfo getBServer();
	std::map< std::string, CgiConfig > getBCgi();
	std::map< std::string, Location > getBLocation();
	int str_is_digit(const std::string &str);

protected:
	template <typename T>
	std::string printPair8(const T &iterator, bool nl);

protected:
    ServerInfo 							BServer;
    std::map< std::string, Location >	BLocation;
	std::map< std::string, CgiConfig >		BCgi;
	bool								redirect;
};

#endif
