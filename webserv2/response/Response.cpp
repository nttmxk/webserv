/***************************************************/
/* CODED BY JIN H. BANG, kbaek ====================*/
/***************************************************/

#include "Response.hpp"

/*
	로직 순서
	1. req 파싱 구간에서 발생한 에러 미리 처리
	2. cgi 관련 response 처리
	3. 일반 get, post, delete 처리
	4. client 의 새로운 request 를 위해 기존 상태는 초기화
*/
bool
Response::cgiFinder(InfoClient &infoClient)
{
	// std::string str = infoClient.req.t_result.path;
	std::string str = "https://robodream.tistory.com/418.py";
	size_t sub;

	if ((sub = str.rfind(".")) != std::string::npos)
		str = str.substr(sub);
	else
		return false;
	if (infoClient._server->Cgi.find(str) == infoClient._server->Cgi.end())
		return false;
	return true;
}

void
Response::responseToClient(int clientSocket, InfoClient infoClient)
{

	// std::cout << "path = " << infoClient.req._target << std::endl;
	if (infoClient.req.t_result.pStatus == Request::pError) {
		makeErrorResponseMsg(infoClient, infoClient.req.t_result.status);
	}
	else if (cgiFinder(infoClient)) {
		// makeCgiResponseMsg(infoClient);
		//cgi 객체 생성후 요청
	}
	else {
		makeResponseMsg(infoClient);
	}
	//sendReseponse(clientSocket);
	infoClient.req.clearRequest();
	
	//(void)infoClient; // to be used

	// std::cout << " response to client : " << clientSocket << "\n";
	// long valWrite = write(clientSocket, resMsg.c_str(), resMsg.size());
	// if (valWrite == (long)resMsg.size())
	// 	std::cout << "SERVER RESPONSE SENT\n";
	// close(clientSocket);
}

void
Response::makeResponseMsg(InfoClient &infoClient)
{
	std::cout << "makeResponseMsg "<< std::endl;
	// int urlVal = urlValidate(infoClient.req.t_result.path);
	// if (urlVal == false)
	// 	return (httpResponse(infoClient.req.t_result.path, 404));

	// if (infoClient.req.t_result.method == Request::GET) {
	// 	int fileFd = open("static/index.html");
	// 	int ret = 1;
	// 	while (ret > 0)
	// 	{
	// 		char fileBuff[1000] = {0};
	// 		ret = read(fileFd, fileBuff, sizeof(fileBuff));
	// 		htmlMsg += fileBuff;
	// 	}
	// }

}

void
Response::makeErrorResponseMsg(InfoClient &infoClient, int errorCode)
{
	std::cout << "makeErrorResponseMsg "<< std::endl;
}


// std::string
// Response::httpRes2XX()
// {}

// std::string
// Response::httpRes3XX()
// {}

// std::string
// Response::httpRes4XX()
// {}

// std::string
// Response::httpRes500()
// {}
