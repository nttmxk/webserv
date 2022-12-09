#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <fstream>
# include <algorithm>
# include <stdexcept>
class Config
{
private:
	std::string path;
	std::string content;
	std::string token;

	int fd;
	int workers;


public:
	Config();
	~Config();
	Config(const char* _path);
	void tokenize();
	void parse();
	std::string buff;
};

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