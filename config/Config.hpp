#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <fstream>
# include <algorithm>
# include <stdexcept>
# include <vector>
# include <map>

#include "Config_struct.hpp"
// using namespace std;

class BaseServer
{
public:

    BaseServer(){};
	BaseServer(const std::string& src);
    ~BaseServer() {};
    
    //  Access Location
	Location& operator[](const std::string& key);
	const std::map<std::string, Location>& getLocations() const;

    //  Access Attribution
	const std::vector<std::string>& BServer(const std::string& key) const;

	bool is_exist(const std::string& key, std::string) const;
	//	data to std::string
	// std::string id();
	// std::string str(size_t tab_size);


protected:
    ServerInfo 							BServer;
    std::map< std::string, Location >	BLocation;
	bool								redirect;
};


struct Config_base
{
protected:
	std::vector<BaseServer> 		base;
	int								numOfServer;
};




class Config : public Config_base
{
private:
	// std::string path;
	// std::string content;
	// std::string token;
	// int fd;
	// int workers;

public:
	Config();
	Config(Config const &src);
	Config &operator=(Config const &srcs);
	~Config();
	Config(const char* _path);
	void tokenize();
	void parse();
	std::string buff;
};

std::ostream &operator<<(std::ostream &ost, const Config &conf);

Config::Config()
{
}

Config::~Config()
{
}


Config::Config(const char* _path)
{
	

}


#endif