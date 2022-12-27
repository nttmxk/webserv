/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <string>

#include "Define.hpp"

class Config
{
	public:
		std::string ipAddr;
		int port;
	public:
		Config(std::string ipAddr, int port)
		: ipAddr(ipAddr), port(port) {}
};

#endif
