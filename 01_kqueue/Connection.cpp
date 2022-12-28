/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#include "Connection.hpp"
#include "Define.hpp"
#include "Response.hpp"

class Response;

Connection::Connection()
{}

Connection::~Connection()
{}

void
Connection::connectionLoop(InfoServer &serverInfo)
{
	_eventManager.declareKqueue();
	_eventManager.enrollEventToChangeList(serverInfo._serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	int eventsNum = 0;
	struct kevent const *currEvent = nullptr;
	while (true)
	{
		eventsNum = _eventManager.senseEvents();
		for (int i = 0; i < eventsNum; ++i)
		{

			currEvent = const_cast<struct kevent const *>(&(_eventManager.getEventList()[i]));

			/* error case */
			if (currEvent->flags & EV_ERROR) {
				if (currEvent->ident == serverInfo._serverSocket)
				{
					std::cerr << "server error : ";
					break ;
				}
				else
				{
					std::cerr << "client error : ";
					close(currEvent->ident);
				}
			}

			/* read event */
			if (currEvent->flags & EVFILT_READ)
			{
				_clientSocket = accept(serverInfo._serverSocket, (sockaddr *)&serverInfo._serverAddr, &serverInfo._serverAddrLen);
				if (_clientSocket == FAIL)
				{
					std::cerr << "Client connection error";
					break;
				}

				char buffer[BUFFER_SIZE] = {0};
				int valRead = read(_clientSocket, buffer, BUFFER_SIZE);
				if (valRead == FAIL)
				{
					std::cerr << "Read error";
					break;
				}

				fcntl(_clientSocket, F_SETFL, O_NONBLOCK);

				_eventManager.clearChangeList();
				_eventManager.enrollEventToChangeList(_clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
				_eventManager.enrollEventToChangeList(_clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
			}

			/* write event */
			if (currEvent->flags & EVFILT_WRITE)
			{
				Response responser;
				responser.responseToClient(_clientSocket, serverInfo);
			}
		}
	}
}
