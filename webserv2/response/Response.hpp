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
	Response() : HttpResInfo(), status(rNone), _result(""), _sentBytes(0), _totalBytes(0) { }
public:
	void	responseToClient(int clientSocket, InfoClient &infoClient);
	void	sendToClient(InfoClient &infoClient);

	void	makeResponseMsg(InfoClient &infoClient);
	void	makeErrorResponseMsg(InfoClient &infoClient, int errorCode);
	void	makeRedirectResponse(InfoClient &infoClient);
	void	startResponse(InfoClient &infoClient);
	void	initResponse(InfoClient &infoClient);
	int		readFd(InfoClient &infoClient, int fd);
	bool	cgiFinder(InfoClient &infoClient);
	bool	redirectionFinder(InfoClient &infoClient);
	void	Get();
	void	Post();
	void	Delete();

	//for write(), send()
	std::string getResult(){return _result;}
	const char*	getSendResult() const;
	size_t		getSendResultSize() const;
	size_t		changePosition(int n);
	void		clearResult();

public:
	std::string makeResponseGET(InfoClient &infoClient);
	std::string makeResponseERR();
private:
	std::string resMsgBody(std::string srcLocation);
	std::string resMsgHeader(InfoClient &infoClient);

private:
		char _fileBuff[1024];
		int _fileFd;
		size_t _sentBytes;
		size_t _totalBytes;

		//InfoClient &infoClient;

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
	rNone = -1,
	rMaking,
	rComplete,
	rError
	};

	enum {
	sError = -1,
	sComplete,
	sSending
	};

	enum {
	wError = -1,
	wComplete,
	wWriting
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
