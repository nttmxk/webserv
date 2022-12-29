/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#include "Connection.hpp"

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
		_eventManager.clearChangeList();
		for (int i = 0; i < eventsNum; ++i)
		{

			currEvent = const_cast<struct kevent const *>(&(_eventManager.getEventList()[i]));

			/* error case */
			if (currEvent->flags & EV_ERROR && errno != EAGAIN) {
				if (currEvent->ident == static_cast<unsigned long>(serverInfo._serverSocket))
				{
					std::cerr << "server error : \n";
					break ;
				}
				else if (currEvent->ident == static_cast<unsigned long>(_clientSocket))
				{
					std::cerr << "client error : \n";
					close(currEvent->ident);
				}
			}

			/* read event */
			if (currEvent->flags & EVFILT_READ)
			{
				_clientSocket = accept(serverInfo._serverSocket, (sockaddr *)&serverInfo._serverAddr, &serverInfo._serverAddrLen);
				if (_clientSocket == FAIL)
				{
					std::cerr << "Client connection error\n";
					break;
				}

				if (fcntl(_clientSocket, F_SETFL, O_NONBLOCK) == FAIL) {
					//throw execption, and send client 5XX
				}

				char buffer[BUFFER_SIZE] = {0};
				int valRead = read(_clientSocket, buffer, BUFFER_SIZE);
				if(valRead == FAIL && errno != EAGAIN)
				{
					std::cerr << "Read error";
					break;
				}
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
