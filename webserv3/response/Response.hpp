/***************************************************/
/* CODED BY JIN H. BANG, kbaek ====================*/
/***************************************************/

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "../includes/Define.hpp"
#include "../connection/InfoServer.hpp"
#include "../connection/InfoClient.hpp"
#include "HttpResInfo.hpp"
#include <sys/stat.h>

class InfoClient;
class HttpResInfo;

class Response : public HttpResInfo
{
public:
<<<<<<< HEAD:webserv2/response/Response.hpp
	Response() : HttpResInfo(), status(PREPARING), _result("") { }

	void	responseToClient(int clientSocket, InfoClient &infoClient);
	void	makeResponseMsg(InfoClient &infoClient);
	void	makeErrorResponseMsg(InfoClient &infoClient, int errorCode);
	void	makeRedirectResponse(InfoClient &infoClient);
	void	startResponse(InfoClient &infoClient);
	void	initResponse(InfoClient &infoClient);
	int		  readFd(InfoClient &infoClient, int fd);
	bool	cgiFinder(InfoClient &infoClient);
	bool	redirectionFinder(InfoClient &infoClient);
	void	Get();
	void	Post();
	void	Delete();
	std::string getResult(){return _result;}
=======
	Response() : HttpResInfo(), status(PREPARING) {}
public:
	void responseToClient(int clientSocket, InfoClient &infoClient);
	void sendToClient();

private:
	void makeResponseMsg(InfoClient &infoClient);
	void makeErrorResponseMsg(InfoClient &infoClient, int errorCode);
	void makeRedirectResponse(InfoClient &infoClient);

	void startResponse(InfoClient &infoClient);

	int	readFd(InfoClient &infoClient, int fd);
	bool cgiFinder(InfoClient &infoClient);
	bool redirectionFinder(InfoClient &infoClient);
	void Get();
	void Post();
	void Delete();
>>>>>>> origin/response_LOGIC_GET:webserv3/response/Response.hpp


	// std::string httpRes2XX();
	// std::string httpRes3XX();
	// std::string httpRes4XX();
	// std::string httpRes500();

public:
	int status;
	std::string fileBuff;

protected:
	std::string _result;
	
// private:
// 	Request &req;
// 	fileEvent &file;


public:
	enum {
	rMaking = 0,
	rComplete,
	rError
	};
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
