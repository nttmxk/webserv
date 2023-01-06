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
				std::cout << "aaaa"  << std::endl;
				if (_serverMap.find(currEvent->ident) != _serverMap.end())
				{
					std::cout << "server read event currEvent->ident = " << currEvent->ident  << std::endl;
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
					infoClient.status = InfoClient::fNone;
					_clientMap.insert(std::pair<int, InfoClient>(clientSocket, infoClient));
					_clientMap[clientSocket].reqMsg = "";
				}

				if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					std::cout << "clientStatus read event = " << currEvent->ident << std::endl;
					char buffer[BUFFER_SIZE] = {0, };

					ssize_t valRead = read(currEvent->ident, &buffer, BUFFER_SIZE);
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
							_clientMap[currEvent->ident].reqMsg.assign(BUFFER_SIZE, 0);
						}
						else {
							Response responser;

							responser.responseToClient(currEvent->ident, _clientMap[currEvent->ident]);
							if (responser.status == Response::rComplete) {
								_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
							}
							else if (responser.status == Response::rError) {
								if (_clientMap[currEvent->ident].status == InfoClient::fMaking) {
									_eventManager.enrollEventToChangeList(_clientMap[currEvent->ident].file.fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
									_fdMap.insert(std::make_pair(_clientMap[currEvent->ident].file.fd, currEvent->ident));
									std::cout << "read agin fd = " << _clientMap[currEvent->ident].file.fd << " currEvent = " << currEvent->ident <<std::endl;;
								}
								else if (_clientMap[currEvent->ident].status == InfoClient::fComplete) {
									_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
								}
							}
						}
					}
				}
				if (_fdMap.find(currEvent->ident) != _fdMap.end())
				{
					
					int status = readFd(infoClient, fd);
					if (status == -1)
					{
						infoClient.status = InfoClient::fError;
						std::cout << "errorPath 22 failier" << std::endl;
					}
					else if (status == 0)
					{
						infoClient.status = InfoClient::fMaking;
						std::cout << "file buffer = " << infoClient.file.buffer << std::endl;
					}
					else
						infoClient.status = InfoClient::fComplete;
					_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_READ, EV_DELETE | EV_DISABLE, 0, 0, NULL);
				}
					
			}

			/* write event */
			else if (currEvent->filter == EVFILT_WRITE)
			{
				_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_DELETE | EV_DISABLE, 0, 0, NULL);
				
				//std::map<int, InfoClient>::iterator it = _clientMap.find(currEvent->ident);

				// Response responser;

				// responser.responseToClient(currEvent->ident, _clientMap[currEvent->ident]);
				// if (responser.status == Response::rComplete || responser.status == Response::rError)
				// {
				// 	_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
				// }
				
			}
		}
	}
}
