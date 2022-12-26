/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#include <Response.hpp>

void
Response::responseToClient(int clientSocket, InfoServer &serverInfo)
{
	std::string resMsg = makeResponseMsg();

	long valWrite = write(clientSocket, resMsg.c_str(), resMsg.size());
	if (valWrite == resMsg.size())
		std::cout << "SERVER RESPONSE SENT\n";
}

std::string
Response::makeResponseMsg()
{
	std::string htmlMsg = "<!DOCTYPE html><html lang=\"en\"><body><h1> Webserv </h1><h2> Hello from Server </h2></body></html>";
	std::ostringstream ss;
	ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlMsg.size() << "\r\n" << htmlMsg;
	return (ss.str());
}
