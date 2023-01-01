/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
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
			// std::cout << "	currEvent fd : " << currEvent->ident << "\n";
			/* error case */
			if (currEvent->flags & EV_ERROR) {
				if (currEvent->ident == static_cast<uintptr_t>(serverInfo._serverSocket))
				{
					std::cerr << "server error : \n";
					break ;
				}
				else if (currEvent->ident == static_cast<uintptr_t>(_clientSocket))
				{
					std::cerr << "client error : \n";
					close(currEvent->ident);
				}
			}

			/* read event */
			else if (currEvent->filter & EVFILT_READ)
			{
				if (currEvent->ident == static_cast<uintptr_t>(serverInfo._serverSocket))
				{
					_clientSocket = accept(serverInfo._serverSocket, (sockaddr *)&serverInfo._serverAddr, &serverInfo._serverAddrLen);
					// std::cout << "	accept client socket : " << _clientSocket << "\n";
					if (_clientSocket == FAIL)
					{
						std::cerr << "Client connection error\n";
						break;
					}
					fcntl(_clientSocket, F_SETFL, O_NONBLOCK);
					_eventManager.enrollEventToChangeList(_clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					_eventManager.enrollEventToChangeList(_clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
				}
				else
				{
					char buffer[BUFFER_SIZE];
					int valRead = read(currEvent->ident, buffer, sizeof(buffer));
					if (valRead == FAIL)
					{
						std::cerr << "Read error";
						exit(1);
					}
					else
					{
						buffer[valRead] = '\0';
						std::cout << "Received data from " << currEvent->ident << ": " << buffer << "\n";
					}
				}
			}

			/* write event */
			else if (currEvent->filter & EVFILT_WRITE)
			{
				Response responser;
				responser.responseToClient(_clientSocket, serverInfo);
			}
		}
	}
}
