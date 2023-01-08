/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "includes/Define.hpp"
#include "includes/cppLibrary.hpp"
#include "../configParser/Config.hpp"
// class Connection; //in Connection.hpp // forward dec -> Field has incomplete type Error
#include "connection/Connection.hpp"

class InfoServer; //in Connection.hpp
class Config; //in Config.hpp

class HttpServer
{
	/* config */
	private:
		std::vector<int> _serverSockets;
		std::vector<InfoServer> _infoServers;
		std::map<int, InfoServer> _serverMap;

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

		class HttpServerError : public std::exception
		{
			public:
				const char *what() const throw()
				{
					return "httpServer Error";
				}
		};
};

#endif
