/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#include "HttpServer.hpp"

HttpServer::HttpServer(Config &config)
{
	_ipAddress = config.ipAddr;
	_port = config.port;
	_serverAddr.sin_family = AF_INET; //ip v4
	_serverAddr.sin_port = htons(_port);
	_serverAddr.sin_addr.s_addr = inet_addr(_ipAddress.c_str()); //inet_addr converts 'char const *' to 'unsigned long' in network byte order
	_serverAddrLen = sizeof(_serverAddr);
	memset(_serverAddr.sin_zero, 0, sizeof(_serverAddr.sin_zero)); //it's a buffer only needed to convert 'sockaddr_in' type to 'sockaddr' type, which is larger type.
}

HttpServer::~HttpServer() {}

/* HttpServer::openServer() creates server side socket for listening */
int
HttpServer::openServer()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket < 0) {
		std::cerr << "Server socket()";
		return (FAIL);
	}

	if (bind(_serverSocket, (sockaddr *)&_serverAddr, _serverAddrLen) != SUCCESS) {
		std::cerr << "Server bind()";
		return (FAIL);
	}

	if (listen(_serverSocket, 128) != SUCCESS) {
		std::cerr << "Server listen()";
		return (FAIL);
	}

	std::cout << "listening on " << _ipAddress << ":" << _port << "\n";

	return (SUCCESS);
}

/* HttpServer::runServer() includes Connection clas object => accept() in loop */
void
HttpServer::runServer()
{
	InfoServer serverInfo;

	serverInfo._serverSocket = _serverSocket;
	serverInfo._ipAddress = _ipAddress;
	serverInfo._port = _port;
	serverInfo._serverAddr = _serverAddr;
	serverInfo._serverAddrLen = _serverAddrLen;
	serverInfo._requestMsg = _requestMsg;

	// _connectManager.makeKqueue();
	_clientManager.connectionLoop(serverInfo);
}

/* HttpServer::closeServer closes all related sockets (server and clients) */
void
HttpServer::closeServer()
{
	close(_serverSocket);
	// close(_clientSocket);
	exit(0);
}

