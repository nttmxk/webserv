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
		int _clientSocket;

	public:
		Connection();
		~Connection();
		void connectionLoop(InfoServer &serverInfo);
};

#endif
