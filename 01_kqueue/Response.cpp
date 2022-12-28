/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#include "Response.hpp"

Response::Response(){}

Response::~Response(){}

void
Response::responseToClient(int clientSocket, InfoServer &serverInfo)
{
	std::string resMsg = makeResponseMsg();
	(void)serverInfo;

	long valWrite = write(clientSocket, resMsg.c_str(), resMsg.size());
	if (valWrite == (long)resMsg.size())
		std::cout << "SERVER RESPONSE SENT\n";
	close(clientSocket);
}

std::string
Response::makeResponseMsg()
{
	std::string htmlMsg = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: 200\n\n<h1>Hello</h1><img src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==\" alt=\"Red dot\" />";
	return (htmlMsg);
}
