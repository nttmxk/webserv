/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/* kqueue */
#include <sys/time.h>
#include <sys/event.h>
#include <sys/types.h>
#include <vector>
#include <iterator>

#include <fcntl.h>

#include "Define.hpp"
#include "Response.hpp"
// class Response;

class InfoServer;
class Multiplex;

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



/***************************************/
/* Classes managed by Connection class */
/***************************************/

/* InfoServer *///-----------------------
class InfoServer
{
	public:
		int _serverSocket;
		std::string _ipAddress;
		int _port;
		struct sockaddr_in _serverAddr;
		unsigned int _serverAddrLen;
		std::string _requestMsg;
	public:
		InfoServer &operator=(InfoServer const &rhs)
		{
			_serverSocket = rhs._serverSocket;
			_ipAddress = rhs._ipAddress;
			_port = rhs._port;
			_serverAddr = rhs._serverAddr;
			_serverAddrLen = rhs._serverAddrLen;
			_requestMsg = rhs._requestMsg;
			return (*this);
		}
};



/* Multiplex *///-----------------------
class Multiplex
{
	private:
		int _kq;
		std::vector<struct kevent> _changeList;
		struct kevent _eventList[MAX_NUM_EVENTS];

	public:
		void declareKqueue();
		void enrollEventToChangeList(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata);
		int senseEvents();
		void clearChangeList();
		struct kevent const *getEventList() const;
};

void
Multiplex::declareKqueue()
{
	_kq = kqueue();
	if (_kq == -1)
	{
		std::cerr << "kqueue() error : ";
		exit(1); // make it throw later
	}
}

void
Multiplex::enrollEventToChangeList(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata)
{
	struct kevent event;

	// EV_SET(&event, ident, filter, flags, fflags, data, udata); //ident = socketFd
	EV_SET(&event, ident, filter, flags, fflags, data, udata);
	_changeList.push_back(event);
}

int
Multiplex::senseEvents()
{
	int sensedEvents;

	sensedEvents = kevent(_kq, &_changeList[0], _changeList.size(), _eventList, MAX_NUM_EVENTS, NULL); // SENSE NEW_EVENTS(연결 감지)
	if (sensedEvents == -1)
	{
		std::cerr << "kevent() error :";
		exit(1); // make it throw later
	}
	return (sensedEvents);
}

void
Multiplex::clearChangeList()
{
	_changeList.clear();
}

struct kevent const *
Multiplex::getEventList() const
{
	return (_eventList);
}

#endif
