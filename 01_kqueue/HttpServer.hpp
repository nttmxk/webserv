/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

#include "Define.hpp"

// class Connection; //in Connection.hpp // forward dec -> Field has incomplete type Error
#include "Connection.hpp"

class InfoServer; //in Connection.hpp
class Config; //in Config.hpp

class HttpServer
{
	/* config */
	private:
		std::string _ipAddress;
		int _port;
	/* made from config */
	private:
		struct sockaddr_in _serverAddr;
		unsigned int _serverAddrLen;
	/* memeber variables */
	private:
		int _serverSocket;
		std::string _requestMsg;
	/* class memebers */
	private:
		Connection _clientManager; // if many servers, make it pointer type

	/* methods */
	public:
		HttpServer(Config &config);
		~HttpServer();
		int openServer();
		void runServer();
		void closeServer();
};


#include "Config.hpp"
#endif
