#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <iostream>
# include <string>
# include <fstream>
# include <algorithm>
# include <stdexcept>
# include <vector>
# include <map>
#include <stdio.h>

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
	BaseServer(const std::string& src) {};
    ~BaseServer() {};
    
    //  Access Location
	// Location& operator[](const std::string& key);
	// const std::map<std::string, Location>& getLocations() const;

    // //  Access Attribution
	// const std::vector<std::string>& BServer(const std::string& key) const;

	// bool is_exist(const std::string& key, std::string) const;
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
public:
	Config();
	Config(Config const &src);
	// Config &operator=(Config const &srcs) {};
	~Config();
	Config(const std::string path);

	class	FileNotFoundException: public std::exception{
		virtual const char	*what() const throw();
	};


private:
	void configInit(const std::string path);
	void configParse();
	// void tokenize();
	// void parse();


private:
	// std::string path;
	// std::string content;
	// std::string token;
	// int fd;
	// int workers;
	std::vector<std::string> file;
};

// std::ostream &operator<<(std::ostream &ost, const Config &conf);

Config::Config()
{}

Config::~Config()
{}

Config::Config(Config const &src)
{}

Config::Config(const std::string _path) {

	try {
		Config::configInit(_path);
		Config::configParse();
	}
	catch (Config::FileNotFoundException &e) {
		std::cerr <<  "Error: couldn't open default config file" << std::endl;
	}
	return ;
}

void	Config::configInit(const std::string _path) {


	std::string buffer;

	std::ifstream ifs(_path);
	if  (!ifs.is_open())
		throw Config::FileNotFoundException();
	while (std::getline(ifs, buffer, '\n'))
	{
		const char *t = " \t\n\r\f\v;";
		buffer.erase(0, buffer.find_first_not_of(t));
		buffer.erase(buffer.find_last_not_of(t) + 1);
		if (buffer.size() > 0)
			file.push_back(buffer);
	}
}

void	Config::configParse()
{
	unsigned int	fileSize;

	fileSize = file.size();
	if (file.empty() || fileSize < 8)
		exit (1);
	for (std::vector<int>::size_type i = 0; i < fileSize; i++)
	{
		if (file[i] == "server") {
			ConfigServer  server;
			++i;
			if (file[i] != "{") {
				std::cerr << "Error: expected '{' after server directive." << std::endl;
				exit 1;
			}
			++i;
			if (!server.parseServer(i, file)) {
				std::cerr << "Error: error in config file [" << filename << "]" ] <<  std::endl;
				exit 1;
			}
			this->_servers.push_back(server);
		}
		else {
				std::cerr << "Error: unknown directive [" << file[i] << "]" ] << std::endl;
				exit 1;
		}
	}



}


const char	*Config::FileNotFoundException::what() const throw(){
	return "Exception thrown: could not open configuration file";
}

#endif