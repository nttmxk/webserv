/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#include "HttpServer.hpp"
#include "includes/Define.hpp"

HttpServer::HttpServer(Config &config)
{
	// config.print_config();
	std::vector<BaseServer> tmpBase = config.getConfigBase();
	std::vector<BaseServer>::iterator it;
	for (it = tmpBase.begin(); it != tmpBase.end(); ++it){
		InfoServer tmpInfo;

		tmpInfo._serverSocket = NONE;
		tmpInfo._ipAddress = it.base()->getBServer().host;
		tmpInfo._port = it.base()->getBServer().port;
		tmpInfo._serverAddr.sin_family = AF_INET; // ip v4
		tmpInfo._serverAddr.sin_port = htons(tmpInfo._port);
		tmpInfo._serverAddr.sin_addr.s_addr = inet_addr(tmpInfo._ipAddress.c_str()); // inet_addr converts 'char const *' to 'unsigned long' in network byte order
		tmpInfo._serverAddrLen = sizeof(tmpInfo._serverAddr);
		memset(tmpInfo._serverAddr.sin_zero, 0, sizeof(tmpInfo._serverAddr.sin_zero)); // it's a buffer only needed to convert 'sockaddr_in' type to 'sockaddr' type, which is larger type.

		_infoServers.push_back(tmpInfo);
	}
}

HttpServer::~HttpServer() {}

/* HttpServer::openServer() creates server side socket for listening */
int
HttpServer::openServer()
{
	std::vector<InfoServer>::iterator it;
	for (it = _infoServers.begin(); it != _infoServers.end(); ++it) {
		it->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (it->_serverSocket < 0)
			throw HttpServerError();

		// Socket resue address option
		int opt = 1;
		if (setsockopt(it->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == FAIL)
			throw HttpServerError();

		if (fcntl(it->_serverSocket, F_SETFL, O_NONBLOCK) == FAIL) {
			std::cerr << "serverSocket fcntl() :";
			return (FAIL);
		}

		if (bind(it->_serverSocket, (sockaddr *)&it->_serverAddr, it->_serverAddrLen) != SUCCESS)
			throw HttpServerError();

		if (listen(it->_serverSocket, 128) != SUCCESS)
			throw HttpServerError();

		_serverMap.insert(std::pair<int, InfoServer>(it->_serverSocket, *it ) );
		_serverSockets.push_back(it->_serverSocket);

		std::cout << "listening on " << it->_ipAddress << ":" << it->_port << "\n";
	}
	return (SUCCESS);
}

/* HttpServer::runServer() includes Connection clas object => accept() in loop */
void
HttpServer::runServer()
{
	_clientManager._serverMap = _serverMap;
	_clientManager._serverSockets = _serverSockets;
	// _clientManager(Connection class) is declared in class HttpServer

	_clientManager.connectionLoop();
}

/* HttpServer::closeServer closes all related sockets (server and clients) */
void
HttpServer::closeServer()
{
	std::vector<InfoServer>::iterator it;
	for (it = _infoServers.begin(); it != _infoServers.end(); ++it)
		close(it->_serverSocket);
	exit(0);
}

