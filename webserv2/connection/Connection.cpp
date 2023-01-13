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
		std::cout << "\n\n****while start*******\n";
		std::cout << "file_map size = " << _fdMap.size() << std::endl;
		std::cout << "client map size = " << _clientMap.size() << std::endl;
		eventsNum = _eventManager.senseEvents();
		_eventManager.clearChangeList();
		for (int i = 0; i < eventsNum; ++i)
		{
			std::cout << "\n***for*** \n eventsNum : " << eventsNum<< std::endl;
			currEvent = const_cast<struct kevent const *>(&(_eventManager.getEventList()[i]));

			/* error case */
			if (currEvent->flags & EV_ERROR)
			{
				if (_serverMap.find(currEvent->ident) != _serverMap.end())
				{
					_serverMap.erase(_serverMap.find(currEvent->ident));
					close(currEvent->ident);
					std::cerr << " server error case \n";
					throw ConnectionError();
				}
				else if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					std::cerr << currEvent->ident << " client error case \n";
					close(currEvent->ident);
					_clientMap.erase(_clientMap.find(currEvent->ident));
					throw ConnectionError();
				}
				// _fdMap.erase(_fdMap.find(currEvent->ident)->first);
				// close(currEvent->ident);
			}
			if (currEvent->filter == EVFILT_TIMER)
			{
				std::cout << "\n\n EVFILT_TIMER : " << currEvent->ident << "\n";
				if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					clearTimeoutedAccess(currEvent->ident);
				}
				std::cout << "\n\n TIMER EVENT DONE-------------------------------------\n";
			}
			/* read event */
			if (currEvent->filter == EVFILT_READ)
			{
				std::cout << "\n\n EVFILT_READ : " << currEvent->ident << std::endl;;
			
				if (_serverMap.find(currEvent->ident) != _serverMap.end())
				{
					std::cout << "SERVER READ EVENT" << std::endl;
					int clientSocket = accept(currEvent->ident, (sockaddr *)&_serverMap[currEvent->ident]._serverAddr, &_serverMap[currEvent->ident]._serverAddrLen);
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
					static const time_t	TIMER = 60;
					_eventManager.enrollEventToChangeList(clientSocket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
					_eventManager.enrollEventToChangeList(clientSocket, EVFILT_TIMER, EV_ADD | EV_ONESHOT, NOTE_SECONDS, TIMER, NULL);

					_serverMap[currEvent->ident]._clients.push_back(clientSocket);
					InfoClient infoClient; // need to be initialized
					infoClient._clientSocket = clientSocket;
					infoClient._server = &_serverMap[currEvent->ident];
					infoClient.status = InfoClient::fNone;
					std::cout << "\n\n INSER CLIENT : " << clientSocket << "\n\n";
					_clientMap.insert(std::pair<int, InfoClient>(clientSocket, infoClient));
					_clientMap[clientSocket].reqMsg = "";

					Response tmpResponser;
					_responserMap.insert(std::pair<int, Response>(clientSocket, tmpResponser));
				}

				if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					std::cout << "CLIENT READ EVENT " << currEvent->ident << std::endl;
					char buffer[BUFFER_SIZE + 1] = {0, };

					//int ret = readSocket(_clientMap[currEvent->ident]);
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
						//std::cout << "buffer = " << buffer << std::endl;
						_clientMap[currEvent->ident].reqMsg = buffer;
						_clientMap[currEvent->ident].req.parseMessage(_clientMap[currEvent->ident].reqMsg);
						if (_clientMap[currEvent->ident].req.t_result.pStatus != Request::pComplete &&
							_clientMap[currEvent->ident].req.t_result.pStatus != Request::pError) {
							_clientMap[currEvent->ident].reqMsg.assign(BUFFER_SIZE, 0);
						}
						else {
							_responserMap[currEvent->ident].responseToClient(currEvent->ident, _clientMap[currEvent->ident]);
							if (_responserMap[currEvent->ident].status == Response::rComplete) {
								std::cout << "Response::rComplete\n";
								_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
							}
							else if (_responserMap[currEvent->ident].status == Response::rError) {
								if (_clientMap[currEvent->ident].status == InfoClient::fMaking) {
									std::cout << "file fd = " <<_clientMap[currEvent->ident].file.fd << std::endl;
									_eventManager.enrollEventToChangeList(_clientMap[currEvent->ident].file.fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
									_fdMap.insert(std::make_pair(_clientMap[currEvent->ident].file.fd, currEvent->ident));
									std::cout << "read agin fd = " << _clientMap[currEvent->ident].file.fd << " currEvent = " << currEvent->ident <<std::endl;;
								}
								else if (_clientMap[currEvent->ident].status == InfoClient::fComplete) {
									std::cout << "InfoClient::fComplete\n";
									_clientMap[currEvent->ident].status = InfoClient::fNone;
									_responserMap[currEvent->ident].status == Response::rNone;
									_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
								}
							}
						}
					}
				}
				if (_fdMap.find(currEvent->ident) != _fdMap.end())
				{
					std::cout << "file event = " << currEvent->ident << std::endl;
					std::cout << _fdMap[currEvent->ident] << std::endl;
					int res = readFile(_clientMap[_fdMap[currEvent->ident]], currEvent->ident);

					switch (res)
					{
					case InfoClient::fError:
						//500 error page open
						std::cout << "fError" << std::endl;//close 했기 때문에 ...처리 별도 필요
						break;
					case InfoClient::fMaking:
						//keep reading
						std::cout << "fMaking = " << _clientMap[_fdMap[currEvent->ident]].file.buffer << std::endl;
						break;
					case InfoClient::fComplete:
						_responserMap[_fdMap[currEvent->ident]].startResponse(_clientMap[_fdMap[currEvent->ident]]);
						//_eventManager.enrollEventToChangeList(_fdMap[currEvent->ident], EVFILT_READ, EV_DELETE | EV_DISABLE, 0, 0, NULL);
						_eventManager.enrollEventToChangeList(_fdMap[currEvent->ident], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
						std::cout << currEvent->ident << " file reading done. open client " << _fdMap[currEvent->ident] << std::endl;;
						close(currEvent->ident);
						_fdMap.erase(_fdMap.find(currEvent->ident)->first);
						break;
					}
				}
				std::cout << "\n\n READ EVENT DONE-------------------------------------\n";
			}

			/* write event */
			 if (currEvent->filter == EVFILT_WRITE)
			{
				std::cout << "\n\n WRITE EVENT : " << currEvent->ident << std::endl;

				if (_clientMap.find(currEvent->ident) != _clientMap.end())
				{
					std::cout << " CLIENT FD write\n";
					//std::cout << _responserMap[currEvent->ident].getResult() << std::endl;

					//check sending file, and if there is a file sending, dont make it go down!!!!!!!

					_responserMap[currEvent->ident].sendToClient(_clientMap[currEvent->ident]);

					switch (_responserMap[currEvent->ident].status)
					{
					case Response::sError:
						std::cout << "Response Error\n";
						if (_clientMap.find(currEvent->ident) != _clientMap.end())
						{
							_clientMap.find(currEvent->ident)->second.clear();
							_clientMap.erase(currEvent->ident);
							close(currEvent->ident);		
						}
						break;
					case Response::sComplete:
						std::cout << "Response Comple\n";
						_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
						_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_WRITE, EV_DELETE | EV_DISABLE, 0, 0, NULL);
						// _responserMap[currEvent->ident].clearResInfo();
						// _responserMap[currEvent->ident].clearResult();
						_responserMap.erase(_responserMap.find(currEvent->ident));
						if (_responserMap.find(currEvent->ident) != _responserMap.end())
							std::cout << "not erased\n\n";
						_clientMap.find(currEvent->ident)->second.clear();
						break;
					case Response::sSending:
						std::cout << "Response Sending\n";
						break;
					}	
				}

				if (_fdMap.find(currEvent->ident) != _fdMap.end())
				{
					std::cout << " FILE FD write\n";
					int currentClient = _fdMap[currEvent->ident];

					int rdFile = writeFile(currEvent->ident);
					switch (rdFile)
					{
					case Response::wError:
						std::cout << "File Write Error\n";
						break;
					case Response::wComplete:
						std::cout << "File Write Complete\n";
						if (_clientMap[currentClient].isCgi == false)
						{
							_eventManager.enrollEventToChangeList(_fdMap[currEvent->ident], EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
						}
						else
						{
							//_responserMap[currentClient].excuteCgi();
							
						}
						std::cout << currEvent->ident << " file write done for : " << _fdMap[currEvent->ident] << std::endl;;
						close(currEvent->ident);
						_fdMap.erase(_fdMap.find(currEvent->ident)->first);
						 
						break;
					case Response::wWriting:
						std::cout << "File Writeing\n";
						break;
			
					}
					

					
				}


				std::cout << "\n\n WRITE EVENT DONE-------------------------------------\n";
			}
			//_eventManager.enrollEventToChangeList(currEvent->ident, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, 60, NULL);
		}
	}
}

int
Connection::writeFile(int fd)
{
	
}

// int
// Connection::writeFile(int fd)
// {
// 	size_t size;

// 	size = write(fd, request.get_body().c_str() + (this->write_size), request.get_body().length() - (this->write_size));
// 	if (size == (size_t)-1)
// 	{
// 		close(this->file_fd);
// 		this->write_size = 0;
// 		return -1;
// 	}
// 	this->write_size += size;
// 	if (this->write_size >= request.get_body().length())
// 	{
// 		close(this->file_fd);
// 		this->write_size = 0;
// 		return 1;
// 	}
// 	return 0;
// }

int
Connection::readSocket(InfoClient &infoClient)
{
	char buffer[BUFFER_SIZE + 1] = {0, };
	int size;

	size = read(infoClient._clientSocket, buffer, BUFFER_SIZE);
	if (size < 0)
	{
		std::cerr << infoClient._clientSocket << " read error case \n";
		return -1;
	}
}



int
Connection::readFile(InfoClient &infoClient, int fd)
{
	char buffer[BUFFER_SIZE + 1];
	
	memset(buffer, 0, sizeof(buffer));

	ssize_t size = read(fd, buffer, sizeof(buffer));
	if (size < 0)
	{
		close(fd);
		_fdMap.erase(fd);
		infoClient.file.buffer.clear();
		return InfoClient::fError;
	}
	infoClient.file.buffer += std::string(buffer, size);
	if (size < BUFFER_SIZE)
	{
		// close(fd);
		// _fdMap.erase(fd);
		return InfoClient::fComplete;
	}
	return InfoClient::fMaking;
}

void
Connection::clearTimeoutedAccess(int socket)
{
	std::cout << "clearTimeoutedAccess" << std::endl;
	if (_clientMap.find(socket) == _clientMap.end())
		return ;
	std::map <int, uintptr_t>::iterator it;
	for (it = _fdMap.begin(); it != _fdMap.end(); it++)
	{
		if (it->second == (uintptr_t)socket)
		{
			_fdMap.erase(it->first);
			continue;
		}
	}
	_clientMap.find(socket)->second.clear();
	_clientMap.erase(socket);
	close(socket);
	_eventManager.enrollEventToChangeList(socket, EVFILT_TIMER, EV_DELETE | EV_DISABLE, 0, 0, NULL);
}


