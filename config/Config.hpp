#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <fstream>
# include <algorithm>
# include <stdexcept>

#include "Config_struct.hpp"

struct Location {

};


struct Server {

};


struct Config_base
{
protected:
	std::vector<Server> 	_servers;
	std::vector<Location>	_location;
};

class Config : public Config_base
{
private:
	std::string path;
	std::string content;
	std::string token;

	int fd;
	int workers;
	//std::vector<ConfigServer> 			_servers;

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