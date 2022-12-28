/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include "Define.hpp"
#include "Connection.hpp"

class InfoServer;

class Response
{
	private:

	public:
		Response();
		~Response();
		void responseToClient(int clientSocket, InfoServer &serverInfo);
		std::string makeResponseMsg();
};


#endif
