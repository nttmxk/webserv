#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Config_struct.hpp"
#include "BaseServer.hpp"
// using namespace std;


/*	Request class
	author	:kbaek
	date 	:2022.12.10

*/
using vector_int_type = std::vector<int>;
std::ostream& operator << (std::ostream& os, const vector_int_type& vect) {
    for (const auto& i : vect)
        os << ' ' << i;
    return os;
}

using vector_str_type = std::vector<std::string>;
std::ostream& operator << (std::ostream& os, const vector_str_type& vect) {
    for (const auto& i : vect)
        os << ' ' << i;
    return os;
}


struct Config_base
{
	Config_base() { numOfServer = 0;}
	std::vector<BaseServer> getConfigBase();
	int	getNumOfServer();
	void print_config();

protected:
	std::vector<BaseServer> 		base;
	int								numOfServer;
};

void Config_base::print_config()
{
	BaseServer baseS;
	ServerInfo tmpserver;
	std::map< std::string, Location >	tmplocation;
	std::map< std::string, CgiConfig >	tmpcgi;
	std::cout << base.size() <<std::endl;
	std::vector<BaseServer>::iterator it;
	for (it = base.begin(); it != base.end(); it++)
	{
		tmpserver = it->getBServer();
		tmplocation = it->getBLocation();
		tmpcgi = it->getBCgi();

		std::cout << "\n\n*** ServerInfo *** \n";
		std::cout << "serverName = [" << tmpserver.serverName << "]\nhost= [" << tmpserver.host << "]\nport = [" << tmpserver.port << \
				"]\nmaxRequestBodySize = [" << tmpserver.maxRequestBodySize << "]\n"<<std::endl;
		std::cout << "\n** Error page ** \n";
		for (auto& item : tmpserver.errorPages)
        	std::cout << "key = [" << item.first << "] value = [" << item.second << "]" <<std::endl;
		
		std::cout << "\n** Location ** \n";
		for (auto& itemL : tmplocation)
		{
        	std::cout << "\nkey = [" << itemL.first << "]"<< std::endl;
			Location tmpL;
			tmpL = itemL.second;
			std::cout << "value = \n";
			std::cout << "	root = [" << tmpL.root << "]\n";
			std::cout << "	maxBody = [" << tmpL.maxBody << "]\n";
			std::cout << "	returnType = [" << tmpL.returnType << "]\n";
			std::cout << "	autoListing = [";
			if (tmpL.autoListing)
				std::cout << "on]\n";
			else
				std::cout << "off]\n";
			std::cout << "	returnRoot = [" << tmpL.returnRoot << "]\n";
			std::cout << "	methods = [" << tmpL.Methods << "]\n";
			std::cout << "	index = [" << tmpL.index << "]\n";
		}

		std::cout << "\n** Cgi Config ** \n";
		for (auto& itemC : tmpcgi)
		{
        	std::cout << "\nkey = [" << itemC.first << "]"<< std::endl;
			CgiConfig tmpC;
			tmpC = itemC.second;
			std::cout << "value = \n";
			std::cout << "	root = [" << tmpC.root << "]\n";
			std::cout << "	methods = [" << tmpC.Methods << "]\n";

		}
	}

}

std::vector<BaseServer> Config_base::getConfigBase()
{
	return this->base;
}
int	Config_base::getNumOfServer()
{
	return this->numOfServer;
}

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

	//int check_bracket(int type, int bracket);

private:
	void configInit(const std::string path);
	void configParse();
	void serverInit(int, int);
	void printV(std::vector <std::string> const &a);
	


private:
	std::vector<std::string> file;
};

std::ostream &operator<<(std::ostream &ost, const Config &conf);

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
		}
		else {
				std::cerr << "Error: Server block syntax error" << std::endl;
				exit (1);
		}
	}
	print_config();

}

void	Config::serverInit(int start, int end)
{
	size_t p;
	size_t sub;
	BaseServer tmpServer;

	std::cout << "i = " << start << "j = " << end << std::endl;
	end += start;
  	for ( ; start < end; start++) 
	{
		if (file[start].find("listen ") != std::string::npos)
		{
			if ((p = file[start].find(":")) != std::string::npos)
			{
				sub = file[start].find(" ") + 1;
				tmpServer.setBServer("h", file[start].substr(sub, p - sub));
				tmpServer.setBServer("p", file[start].substr(p + 1));
			}
			else
			{
				std::cerr << "Error: listen block : syntax error" << std::endl;
				exit (1);
			}
		}	
		else if (file[start].find("server_name ") != std::string::npos)
		{
			sub = file[start].find(" ") + 1;
			tmpServer.setBServer("s", file[start].substr(sub));
		}
		else if (file[start].find("error_page ") != std::string::npos)
		{
			std::vector<int> tmpErrorNum;
			size_t slash;

			sub = file[start].find(" ") + 1;
			if ((slash = file[start].find('/')) == std::string::npos)
				slash = file[start].size();
			else
				slash -= 1;
			std::string subS = file[start].substr(sub, slash - sub);
			std::stringstream ss(subS);
			std::string temp;
			while (getline(ss, temp, ' '))
			{
				int i;
				std::stringstream ssInt(temp);
				ssInt >> i;
				// std::cout << "sub num = ["<< i <<"]" << std::endl;
				tmpErrorNum.push_back(i);
			}
			tmpServer.setError(slash == file[start].size() ? "": file[start].substr(slash + 1), tmpErrorNum);
		}
		else if (file[start].find("location ") != std::string::npos)
		{
			//std::map< std::string, Location >	BLocation;
			sub = file[start].find(" ") + 1;
			size_t q;
			std::string str;
			std::string tmp;
			
			if ((q = file[start].find(" {")) == std::string::npos)
			{
				std::cerr << "Error: Location block { } syntax error" << std::endl;
				exit (1);
			}
			str = file[start].substr(sub, q - sub);
			if(str.empty())
			{
				std::cerr << "Error: Location block / syntax error" << std::endl;
				exit (1);
			}
			int flag;
			Location location;
			location.root = "";
			location.maxBody = 9000;
			location.returnType = -1;
			location.autoListing = false;
			location.returnRoot = "";
			while (flag = (file[++start] != "}"))
			{
				if ((sub = file[start].find(" ")) != std::string::npos)
				{
					tmp = file[start].substr(0, sub);
					if (tmp == "root")
						location.root = file[start].substr(sub + 1);
					else if (tmp == "method")
					{
						std::vector<std::string> methods;
						std::string subS =  file[start].substr(sub + 1);
						std::stringstream ss(subS);
						std::string temp;
						while (getline(ss, temp, ' '))
							methods.push_back(temp);
						location.Methods = methods;
					}
					else if (tmp == "index")
					{
						std::vector<std::string> methods;
						std::string subS =  file[start].substr(sub + 1);
						std::stringstream ss(subS);
						std::string temp;
						while (getline(ss, temp, ' '))
							methods.push_back(temp);
						location.index = methods;
					}
					else if (tmp == "max_body")
					{
						int i;
						std::stringstream ssInt(file[start].substr(sub + 1));
						ssInt >> i;
						location.maxBody = i;
					}
					else if (tmp == "returnType")
					{
						int i;
						std::stringstream ssInt(file[start].substr(sub + 1));
						ssInt >> i;
						location.returnType = i;
					}
					else if (tmp == "autoindex")
					{
						if (file[start].substr(sub + 1) == "on")
							location.autoListing = true;
					}
					else if (tmp == "return")
						location.returnRoot = file[start].substr(sub + 1);
					else
					{
						std::cerr << "Error: Location block has wrong syntax error" << std::endl;
						exit (1);
					}
				}
				else
				{
					std::cerr << "Error: Location block key value syntax error" << std::endl;
					exit (1);
				}
			}
			if (flag)
			{
				std::cerr << "Error: Location block {  } syntax error" << std::endl;
				exit (1);
			}
			tmpServer.setBlcation(str, location);
		}
		else if (file[start].find("cgi ") != std::string::npos)
		{
			sub = file[start].find(" ") + 1;
			size_t q;
			std::string str;
			std::string tmp;
			
			if ((q = file[start].find(" {")) == std::string::npos)
			{
				std::cerr << "Error: Location block { } syntax error" << std::endl;
				exit (1);
			}
			str = file[start].substr(sub, q - sub);
			if(str.empty())
			{
				std::cerr << "Error: Location block / syntax error" << std::endl;
				exit (1);
			}
			
			int flag;
			CgiConfig cgi;
			while (flag = (file[++start] != "}"))
			{
				if ((sub = file[start].find(" ")) != std::string::npos)
				{
					tmp = file[start].substr(0, sub);
					if (tmp == "root")
						cgi.root = file[start].substr(sub + 1);
					else if (tmp == "method")
					{
						std::vector<std::string> methods;
						std::string subS =  file[start].substr(sub + 1);
						std::stringstream ss(subS);
						std::string temp;
						while (getline(ss, temp, ' '))
							methods.push_back(temp);
						cgi.Methods = methods;
					}
				}
				else
				{
					std::cerr << "Error: Location block key value syntax error" << std::endl;
					exit (1);
				}
			}
			if (flag)
			{
				std::cerr << "Error: Location block {  } syntax error" << std::endl;
				exit (1);
			}
			tmpServer.setBCgi(str, cgi);
		}
		else if (file[start] != "server {" && file[start] != "}")
		{
			std::cout <<  "wrong ["<<file[start] << "]" << std::endl;
			std::cerr << "Error: Config block syntax key error" << std::endl;
			exit (1);
		}
	}
	base.push_back(tmpServer);
	numOfServer += 1;
	/* checkfillup();
		1. post is digit
		2. check there are all pare
		3. check there is at least one location block every server
		4. { } 확인

	*/
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