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


/*	Request class
	author	:kbaek
	date 	:2022.12.10
*/

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
	Config init(const char* _path);
	void tokenize();
	void parse();
	std::string buff;

	class	FileNotFoundException: public std::exception{
		virtual const char	*what() const throw();
	};
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

Config::Config(const char*	_path) {

	try {
		Config::init(_path);
	}
	catch (Config::FileNotFoundException &e) {
		std::cerr <<  "Error: couldn't open default config file" << std::endl;
	}
	return ;
}

File	init(const char *filename) {
	int							ret = READER_BUFFER_SIZE;
	char						buffer[READER_BUFFER_SIZE + 1];
	std::string					line = "";
	int							fd;
	File					file;

	for (int i = 0; i < READER_BUFFER_SIZE + 1;  i++)
		buffer[i] = '\0';
	if ((fd = open(filename, O_RDONLY)) <= 0)
		throw ConfigReader::FileNotFoundException();

	for (ret = READER_BUFFER_SIZE; ret > 0;\
	ret = read(fd, buffer,READER_BUFFER_SIZE )) {
		buffer[ret] = '\0';
		line += buffer;
	}

	//	Get the last line, which is ignored otherwise
	if (ret == -1) {
		std::cerr << "Error: couldn't reading config file" <<std::endl;
		return file;
	}
	file = ConfigReader::split(line, std::string(" \n\t"));
 	return file;
}
#endif