#ifndef INFOCLIENT_HPP
#define INFOCLIENT_HPP

#include "InfoServer.hpp"
#include "../includes/cppLibrary.hpp"
#include "../includes/Define.hpp"
#include "../../parseRequest/Request.hpp"

class Response;

class InfoClient
{
	struct fileEvent
	{
		int fd;
		std::size_t size;
		std::string buffer;
		fileEvent() : fd(-1), size(0), buffer(""){}
	};
	public:
		enum {
			fNone = 0,
			fError,
			fComplete,
			fMaking
		};
	public:
		int _clientSocket;
		fileEvent file;
		InfoServer *_server;
		std::string reqMsg;
		Request req;
		int status;
		bool isCgi;



	public:
		InfoClient()
		: _clientSocket(-1), _server(nullptr), reqMsg(""), isCgi(false) {}
		void clear() {
			file.buffer.clear();
			file.fd = -1;
			file.size = 0;
			reqMsg = "";
			req.clearRequest();
			status =fNone;
			
		};
};


#endif
