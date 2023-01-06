/***************************************************/
/* CODED BY JIN H. BANG, kbaek ====================*/
/***************************************************/

#include "Response.hpp"


/*
	로직 순서
	1. req 파싱 구간에서 발생한 에러 미리 처리
	2. redirection 처리
	3. cgi 관련 response 처리
	4. 일반 get, post, delete 처리
	5. client 의 새로운 request 를 위해 기존 상태는 초기화
*/

void
Response::responseToClient(int clientSocket, InfoClient infoClient)
{

	// std::cout << "path = " << infoClient.req._target << std::endl;
	if (infoClient.req.t_result.pStatus == Request::pError) {
		makeErrorResponseMsg(infoClient, infoClient.req.t_result.status);
	}
	else if (redirectionFinder(infoClient)) {
		// makeRedirectResponse(infoClient);
		std::cout << "redirect message\n\n";
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

// int
// vecterIntExist(std::vector <int> const &a) {
// 	for(int i=0; i < a.size(); i++)
// 	{
// 		if (a.at(i))
			
// 	}
// 		std::cout << a.at(i) << '\n';
// 	std::cout << "\n\n\n";
// }
/*

/*
 	1.해당 errorCode 에 대한 파일을 서버에서 가지고 있는 경우
	2. 없는 경우

*/
void
Response::makeErrorResponseMsg(InfoClient &infoClient, int errorCode)
{
	std::cout << "makeErrorResponseMsg "<< std::endl;
	
	std::string errorPath = "";

	//1. config 파일 내부에 해당 error page 가 설정된 경우
	std::map<std::string, std::vector<int> >::iterator it;
	for (it = infoClient._server->errorPages.begin(); it != infoClient._server->errorPages.end(); it++)
	{
		for (unsigned int i = 0; i < it->second.size(); i++ )
		{
			if (it->second.at(i) == errorCode)
				errorPath = it->first;
		}
	}
	if (errorPath != "")
	{
		int fd;
		struct stat ss;
		if (stat(errorPath.c_str(), &ss) == -1 || S_ISREG(ss.st_mode) != true ||
			(fd = open(errorPath.c_str(), O_RDONLY) == -1))
			this->status = errorCaseOne();
		else
		{
			fcntl(fd, F_SETFL, O_NONBLOCK);
			// 1.파일 읽기에 대한 kq 이벤트 등록 
			// 2. file 정보 저장
			if (readFd(fd) == -1)
				this->status = errorCaseOne();
			// close(fd);


		}
		
		
			
	}
	else
	{

	}
	//2. 없는 경우
}

int
Response::readFd(int fd)
{
	char buffer[4096] = {0, };
	ssize_t size = read(fd, buffer, 4096);
	if (size == -1)
	{
		close(fd);
		return -1;
	}
	if (size > 0)
	{
		
	}
	


}

ResponseManager::IoFdPair ResponseManager::HandleGetErrorFailure(void) {
  result_.status = 500;  // INTERNAL_SERVER_ERROR
  is_keep_alive_ = false;
  response_buffer_.content = LAST_ERROR_DOCUMENT;
  io_status_ = SetIoComplete(IO_COMPLETE);
  return IoFdPair();
}

ResponseManager::IoFdPair ResponseManager::GenerateDefaultError(void) {
  std::stringstream ss;
  ss << result_.status << " " << g_status_map[result_.status];
  response_buffer_.content = "<!DOCTYPE html><title>" + ss.str() +
                             "</title><body><h1>" + ss.str() +
                             "</h1></body></html>";
  router_result_.error_path = "default_error.html";
  io_status_ = SetIoComplete(IO_COMPLETE);
  result_.ext = "html";
  return IoFdPair();
}

void
Response::makeResponseMsg(InfoClient &infoClient)
{
	std::cout << "makeResponseMsg "<< std::endl;
	makeErrorResponseMsg(infoClient, 409);
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

bool
Response::cgiFinder(InfoClient &infoClient)
{
	// std::string str = infoClient.req.t_result.path;
	std::string str = "https://robodream.tistory.com/418";
	size_t sub;

	if ((sub = str.rfind(".")) != std::string::npos)
		str = str.substr(sub);
	else
		return false;
	if (infoClient._server->Cgi.find(str) == infoClient._server->Cgi.end())
		return false;
	return true;
}

bool
Response::redirectionFinder(InfoClient &infoClient)
{
	// std::string str = infoClient.req.t_result.path;
	std::string str = "https://robodream.tistory.com/redirectaa";
	size_t sub;

	if ((sub = str.rfind("/")) != std::string::npos)
		str = str.substr(sub);
	else
		return false;

	std::map< std::string, Location >::iterator it;
	for (it = infoClient._server->Location.begin(); it != infoClient._server->Location.end(); it++)
	{
		if (it->first == str && it->second.returnType == 301)
			return true;
	}
	return false;
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
