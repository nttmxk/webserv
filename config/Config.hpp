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

// void BaseServer::printStruct()
// {
// 	std::cout << "ServerInfo : \n";
// 	std::cout << "serverName = " << BServer.serverName << "\nhost= " << BServer.host << "\nport = " << BServer.port << "\nmaxRequestBodySize = " << BServer.maxRequestBodySize << std::endl;

// 	std::cout << "location : \n";
// 	 std::map< std::string, Location>::iterator it;
// 	for (it = BLocation.begin(); it != BLocation.end(); it++)
// 		std::cout << it->first << ' ' <<it->second << std::endl;
// }

struct Config_base
{
	Config_base() { numOfServer = 0;}
	std::vector<BaseServer> getConfigBase();
	int	getNumOfServer();
	// print_config_base(std::vector<BaseServer> 	base, int numOfServer)
	// {
	// 	for (int i = 0; numOfServer > i; i++)
	// 	{
	// 		std::cout << "ServerInfo : \n";
	// 		base.data
	// 	}
	// }
protected:
	std::vector<BaseServer> 		base;
	int								numOfServer;
};
std::vector<BaseServer> Config_base::getConfigBase()
{
	return this->base;
}
int	Config_base::getNumOfServer()
{
	return this->numOfServer;
}
// void Config_base::printStruct()
// {
// 	std::cout << "ServerInfo : \n";
// 	std::cout << "serverName = " << BServer.serverName << "\nhost= " << BServer.host << "\nport = " << BServer.port << "\nmaxRequestBodySize = " << BServer.maxRequestBodySize << std::endl;

// 	std::cout << "location : \n";
// 	 std::map< std::string, Location>::iterator it;
// 	for (it = BLocation.begin(); it != BLocation.end(); it++)
// 		std::cout << it->first << ' ' <<it->second << std::endl;
// }



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
			// std::cout << file[i] << std::endl;
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
			Location tmpLo;
			
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
			// = (file[++start] != "}");
			// // int i = flag == true ? 1 : 0;
			// std::cout << "i === " << i << std::endl;

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
					std::cout <<"tmp = [" << tmp << "]" << std::endl;
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
			int flag;
			while (flag = (file[++start] != "}"))
			{
				
			}
			if (flag)
			{
				std::cerr << "Error: Location block {  } syntax error" << std::endl;
				exit (1);
			}
		}
		else if (file[start] != "server {" && file[start] != "}")
		{
			std::cout <<  "wrong ["<<file[start] << "]" << std::endl;
			std::cerr << "Error: Config block syntax key error" << std::endl;
			exit (1);
		}
		// std::cout << "start = " << start<< "start + end = "<< start + end<< std::endl;
	}
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


// int	check_bracket(int type, int quotat)
// {
// 	if (type == DOUBLE)
// 	{
// 		if (quotat == 0)
// 			quotat = DOUBLE;
// 		else if (quotat == DOUBLE)
// 			quotat = 0;
// 	}
// 	else if (type == SINGLE)
// 	{
// 		if (quotat == 0)
// 			quotat = SINGLE;
// 		else if (quotat == SINGLE)
// 			quotat = 0;
// 	}
// 	return (quotat);
// }


const char	*Config::FileNotFoundException::what() const throw(){
	return "Exception thrown: could not open configuration file";
}

#endif