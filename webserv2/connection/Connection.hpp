/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include "../includes/Define.hpp"
#include "../includes/cppLibrary.hpp"

#include "../response/Response.hpp"
#include "Multiplex.hpp"
#include "InfoServer.hpp"
#include "InfoClient.hpp"

class Connection
{
	private:
		Multiplex _eventManager;

	public:
		std::vector<int> _serverSockets;
		std::map<int, InfoServer> _serverMap;
		std::map<int, InfoClient> _clientMap;
		std::map <int, uintptr_t> _fdMap;
		std::map<int, Response> _responserMap;

	public:
		std::string _clientReq;

	public:
		Connection();
		~Connection();
		void connectionLoop();
		int readSocket(InfoClient &infoClient);
		int	readFile(InfoClient &infoClient, int fd);
		int writeFile(int fd);
		void clearTimeoutedAccess(int socket);
		class ConnectionError : public std::exception
		{
			public:
				const char *what() const throw() {
					return "Connection Error";
				}
		};
};

#endif
