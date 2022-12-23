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

    BaseServer(){
		BServer.serverName = "";
		BServer.host = "";
		BServer.port = "";
		BServer.maxRequestBodySize = -1;
	};
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

	void setBServer(std::string fo, std::string str);
protected:
    ServerInfo 							BServer;
    std::map< std::string, Location >	BLocation;
	bool								redirect;
};

void BaseServer::setBServer(std::string fo, std::string str)
{
	if (fo == "host")
		BServer.host = str;
	else if (fo == "port")
		BServer.port = str;
	else if (fo == "servername")
		BServer.serverName = str;
	// else
	// 	BServer.maxRequestBodySize = atoi(str);
	std::cout << "str = " << str <<std::endl;
}

struct Config_base
{
	Config_base() { numOfServer = 0;}
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
	void serverInit(int, int);
	void printV(std::vector <std::string> const &a);


private:
	std::vector<std::string> file;
};

// std::ostream &operator<<(std::ostream &ost, const Config &conf);

Config::Config()
{}

Config::~Config()
{}

Config::Config(Config const &src)
{}

Config::Config(const std::string _path) : Config_base() {

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
	// printV(file);
}

void	Config::configParse()
{
	unsigned int	fileSize;
	int 			flag = 0;

	fileSize = file.size();
	if (file.empty() || fileSize < 8)
		exit (1);
	std::vector<int>::size_type i = 0;
	while (i < fileSize)
	{
		std::cout << file[i] << std::endl;
		if (file[i] == "server {") {
			int j = 0;
			flag = 1;
			i++;
			j++;
			while (i < fileSize && !(file[i] == "server {"))
			{
				if ((file[i].find("}") != std::string::npos) || ((file[i].find("{") != std::string::npos)))
					flag +=1;
				i++;
				j++;
			}
			if ((flag % 2) == 1) {
				std::cerr << "Error: Server block { } syntax error" << std::endl;
				exit (1);
			}
			serverInit(i - j, j);
			std::cout << file[i] << std::endl;
		}
		else {
				std::cerr << "Error: Server block syntax error" << std::endl;
				exit (1);
		}
	}
}

void	Config::serverInit(int start, int end)
{
	size_t p;
	BaseServer tmpServer;

	numOfServer += 1;
	std::cout << "i = " << start << "j = " << end << std::endl;
	end += start;
  	for ( ; start < end; start++) 
	{
		if (file[start].find("listen ") != std::string::npos)
		{
			if ((p = file[start].find(":")) != std::string::npos)
			{
				size_t sub = file[start].find(" ");
				tmpServer.setBServer("host", file[start].substr(sub, p - sub));
				tmpServer.setBServer("post", file[start].substr(p + 1));
			}
			else
			{
				std::cerr << "Error: listen block : syntax error" << std::endl;
				exit (1);
			}
		}	
		else if (file[start].find("server_name ") != std::string::npos)
		{

		}
		else if (file[start].find("error_page ") != std::string::npos)
		{

		}
		else if (file[start].find("location ") != std::string::npos)
		{

		}
		else if (file[start].find("cgi ") != std::string::npos)
		{

		}
		std::cout << "start = " << start<< "start + end = "<< start + end<< std::endl;
	}
	// checkfillup();
}
void Config::printV(std::vector <std::string> const &a) {


	for(int i=0; i < a.size(); i++)
		std::cout << a.at(i) << '\n';
	std::cout << "\n\n\n";
}

const char	*Config::FileNotFoundException::what() const throw(){
	return "Exception thrown: could not open configuration file";
}

#endif