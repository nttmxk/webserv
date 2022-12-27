/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Define.hpp"
#include "Response.hpp"
// class Response;

class InfoServer
{
	public:
		int _serverSocket;
		std::string _ipAddress;
		int _port;
		struct sockaddr_in _serverAddr;
		unsigned int _serverAddrLen;
		std::string _requestMsg;
	public:
		InfoServer &operator=(InfoServer const &obj)
		{
			_serverSocket = obj._serverSocket;
			_ipAddress = obj._ipAddress;
			_port = obj._port;
			_serverAddr = obj._serverAddr;
			_serverAddrLen = obj._serverAddrLen;
			_requestMsg = obj._requestMsg;
			return (*this);
		}
};

class Connection
{
	private:
		InfoServer _serverInfo;
		int _clientSocket;

	public:
		Connection();
		~Connection();
		void connectionLoop(InfoServer &serverInfo);
};


#endif
