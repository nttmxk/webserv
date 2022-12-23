#ifndef CONFIG_STRUCT_HPP
# define CONFIG_STRUCT_HPP

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Config.hpp"

struct Location
{
	typedef bool Autoindex_type;
  	const Autoindex_type off = false;
  	const Autoindex_type on = true;
    typedef Autoindex_type Autoindex;

	std::string root;
	std::vector<std::string> Methods;
	std::vector<std::string> index;
	Autoindex listing;
	int maxBody;
	int returnType;

};


struct ServerInfo
{
	std::string serverName;
	std::map<std::string, std::string> errorPages;
	std::string host;
	std::string port;
	int maxRequestBodySize;

};


#endif