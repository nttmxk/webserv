/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Define.hpp"
#include "cppLibrary.hpp"

class ServerConf {
	public:
		std::string ipAddr;
		int port;
		std::string serverName;

	public:
		ServerConf(std::string ipAddr, int port, std::string serverName)
		: ipAddr(ipAddr), port(port), serverName(serverName) {}
};

class Config
{
	public:
		std::vector<ServerConf> serverConfs;
		int serverNum;
};

#endif
