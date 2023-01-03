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
					std::cerr << "server error\n";
					break ;
				}
				else
				{
					std::cerr << "client error\n";
					close(currEvent->ident);
					_clientsMap.erase(currEvent->ident);
				}
			}

			/* read event */
			else if (currEvent->filter == EVFILT_READ)
			{
				if (currEvent->ident == static_cast<uintptr_t>(serverInfo._serverSocket))
				{
					int clientSocket = -1;
					clientSocket = accept(serverInfo._serverSocket, (sockaddr *)&serverInfo._serverAddr, &serverInfo._serverAddrLen);
					// std::cout << "	accept client socket : " << clientSocket << "\n";
					if (clientSocket == FAIL)
					{
						std::cerr << "Client connection error\n";
						break;
					}
					fcntl(clientSocket, F_SETFL, O_NONBLOCK);
					_eventManager.enrollEventToChangeList(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					_eventManager.enrollEventToChangeList(clientSocket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
					_clientsMap.insert(std::pair<int, std::string>(clientSocket, ""));
				}
				else if (_clientsMap.find(currEvent->ident)!= _clientsMap.end())
				{
					// char buffer[BUFFER_SIZE];
					char buffer[1];
					int valRead = read(currEvent->ident, buffer, sizeof(buffer));
					if (valRead <= 0)
					{
						if (valRead < 0) {
							std::cerr << "Read error";
						}
						close(currEvent->ident);
						_clientsMap.erase(currEvent->ident);
					}
					else
					{
						buffer[valRead] = '\0';
						_clientsMap[currEvent->ident] += buffer;
						std::cout << "Received data from " << currEvent->ident << ": " << _clientsMap[currEvent->ident] << "\n";
					}
				}
			}

			/* write event */
			else if (currEvent->filter == EVFILT_WRITE)
			{
				std::map<int, std::string>::iterator it = _clientsMap.find(currEvent->ident);
				if (it != _clientsMap.end()) {
					Response responser;
					if (it->second == "GET") {
						//parsing needed
						it->second = "";
						responser.responseToClient(currEvent->ident, serverInfo);
					}
				}
			}
		}
	}
}
