#ifndef CONFIG_STRUCT_HPP
# define CONFIG_STRUCT_HPP

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Config.hpp"

struct CgiConfig
{
	std::string root;
	std::vector<std::string> Methods;
};


struct Location
{
	std::string root;
	std::vector<std::string> Methods;
	std::vector<std::string> index;
	int maxBody;
	int returnType;
	bool autoListing;
	std::string returnRoot;
};


struct ServerInfo
{
	std::string serverName;
	std::map<std::string, std::vector<int> > errorPages;
	std::string host;
	int port;
	int maxRequestBodySize;

};

#endif
