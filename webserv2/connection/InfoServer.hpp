/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef INFOSERVER_HPP
#define INFOSERVER_HPP

#include "../includes/cppLibrary.hpp"
#include "../../configParser/Config_struct.hpp"

/* InfoServer *///-----------------------
class InfoServer 
{
	public:
		int _serverSocket;
		std::string _ipAddress;
		int _port;
		struct sockaddr_in _serverAddr;
		unsigned int _serverAddrLen;
		std::string _requestMsg;
		std::list<int> _clients;

		//config
		std::map<std::string, std::vector<int> > errorPages;
		std::map< std::string, Location >	Location;
		std::map< std::string, CgiConfig >	Cgi;

		//file

	public:
		InfoServer &operator=(InfoServer const &rhs)
		{
			memset(&_serverAddr, 0, sizeof(sockaddr_in));
			_serverSocket = rhs._serverSocket;
			_ipAddress = rhs._ipAddress;
			_port = rhs._port;
			_serverAddr = rhs._serverAddr;
			_serverAddrLen = rhs._serverAddrLen;
			_requestMsg = rhs._requestMsg;
			return (*this);
		}
};

#endif
