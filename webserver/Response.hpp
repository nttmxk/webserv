/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Define.hpp"
#include "InfoServer.hpp"
#include "HttpResInfo.hpp"

class Response : public HttpResInfo
{
	public:
		void responseToClient(int clientSocket, InfoServer &serverInfo);
		std::string makeResponseMsg();
		std::string make2XXRes();
		std::string make4XXRes();
		std::string make5XXRes();
};

#endif



/*
void
Account::_displayTimestamp( void )
{
	std::time_t timeBuff = std::time(NULL);
	struct tm	*time = std::localtime(&timeBuff);

	std::cout << "[" << 1900 + time->tm_year << 1 + time->tm_mon << time->tm_mday
		<< "_" << time->tm_hour << time->tm_min << time->tm_sec << "] ";
}

*/
