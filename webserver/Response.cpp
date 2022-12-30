/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
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
	std::stringstream httpRes;

	/*
	127.0.0.1:8070/user/ -> user.html
	127.0.0.1:8070/user/info -> userinfo.html
	127.0.0.1:8070/user/pic -> userpic.html
	*/

	//req 인포 받아왔다고 가정
	if (req.statusCode == 200 && req.url == "/")
	{
		/* setting */
	}

	/* make response msg */
	// httpRes << getHttpVersion() << " " << getStatusCode() << " " << getStatusMsg() << "\n";
	// httpRes << "Content-Type"

	std::string htmlMsg = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\nContent-Length: 200\n\n<h1>Hello</h1><img src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==\" alt=\"Red dot\" />";
	return (htmlMsg);
}
