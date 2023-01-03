/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP


#include "Define.hpp"
#include "cppLibrary.hpp"

#include "Response.hpp"
#include "Multiplex.hpp"
#include "InfoServer.hpp"


class Connection
{
	private:
		InfoServer _serverInfo;
		Multiplex _eventManager;
		std::map<int, std::string> _clientsMap; // map for client socket:data
		std::string _clientReq;

	public:
		Connection();
		~Connection();
		void connectionLoop(InfoServer &serverInfo);
};

#endif
