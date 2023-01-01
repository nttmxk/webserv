/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Define.hpp"
#include "InfoServer.hpp"

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
