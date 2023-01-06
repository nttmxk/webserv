/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#include "Connection.hpp"

// class Response;

Connection::Connection()
{
}

Connection::~Connection()
{
}

void Connection::connectionLoop()
{
	_eventManager.declareKqueue();

	std::cout << "	::map size : " << _serverMap.size() << "\n"
			  << "	::serverSockets: " << _serverSockets.size() << "\n";

	std::vector<int>::iterator it;
	for (it = _serverSockets.begin(); it != _serverSockets.end(); ++it)
	{
		_eventManager.enrollEventToChangeList(_serverMap[*it]._serverSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
		std::cout << "	enrolled serv socket : " << *it << "\n";
	}

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
			if (currEvent->flags & EV_ERROR)
			{
				if (_serverMap.find(currEvent->ident) != _serverMap.end())
				{
					// close(currEvent->ident);
					std::cerr << " server error case \n";
					throw ConnectionError();
				}
				else if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					std::cerr << " client error case \n";
					close(currEvent->ident);
					_clientMap.erase(currEvent->ident);
					throw ConnectionError();
				}
			}

			/* read event */
			else if (currEvent->filter == EVFILT_READ)
			{
				if (_serverMap.find(currEvent->ident) != _serverMap.end())
				{
					int clientSocket = accept(currEvent->ident, (sockaddr *)&_serverMap[currEvent->ident]._serverAddr, &_serverMap[currEvent->ident]._serverAddrLen);
					// std::cout << "	accepted client socket : " << clientSocket << "\n"; // test code
					if (clientSocket == FAIL)
					{
						std::cerr << " Error : accept() \n";
						throw ConnectionError();
					}

					if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == FAIL)
					{
						std::cerr << " Error : fcntl() \n";
						throw ConnectionError();
					}

					_eventManager.enrollEventToChangeList(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

					_serverMap[currEvent->ident]._clients.push_back(clientSocket);
					InfoClient infoClient; // need to be initialized
					infoClient._clientSocket = clientSocket;
					infoClient._server = &_serverMap[currEvent->ident];
					_clientMap.insert(std::pair<int, InfoClient>(clientSocket, infoClient));
					_clientMap[clientSocket].reqMsg = "";
				}

				else if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					// char buffer[BUFFER_SIZE] = {0};
					char buffer[1024] = {0, };
					// std::cout << "	clientMap size : " << _clientMap.size() << "\n";

					ssize_t valRead = read(currEvent->ident, &buffer, 1024);
					if (valRead == FAIL)
					{
						std::cerr << " from client " << currEvent->ident;
						std::cerr << " Error : read() \n";
						// send error page
						close(currEvent->ident);
						_clientMap.erase(currEvent->ident);
					}
					else
					{
						_clientMap[currEvent->ident].reqMsg = buffer;
						_clientMap[currEvent->ident].req.parseMessage(_clientMap[currEvent->ident].reqMsg);
						if (_clientMap[currEvent->ident].req.t_result.pStatus != Request::pComplete &&
							_clientMap[currEvent->ident].req.t_result.pStatus != Request::pError) {
							_clientMap[currEvent->ident].reqMsg.assign(1024, 0);
						}
						else { //클라이언트로부터 메시지를 모두 전달 받으면 response 를 위해 해당 클라이언트의 읽기 감지는 중단하고, 쓰기 감지는 실행한다.
							_clientMap[currEvent->ident].req.printRequest();
							_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_READ, EV_DELETE | EV_DISABLE, 0, 0, NULL);
							_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
						}
					}
				}
			}

			/* write event */
			else if (currEvent->filter == EVFILT_WRITE)
			{
				_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_DELETE | EV_DISABLE, 0, 0, NULL);

				std::map<int, InfoClient>::iterator it = _clientMap.find(currEvent->ident);

				Response responser;

				responser.responseToClient(currEvent->ident, _clientMap[currEvent->ident]);
				if (responser.status == Response::rComplete || responser.status == Response::rError)
				{
					_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
				}
				
			}
		}
	}
}
