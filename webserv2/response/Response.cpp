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
Response::responseToClient(int clientSocket, InfoClient &infoClient)
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
		infoClient.isCgi = true;
		
		std::cout << "cgiFinder message\n\n";
		// makeCgiResponseMsg(infoClient);
		//cgi 객체 생성후 요청
	}
	else { //get
		makeErrorResponseMsg(infoClient, 404);
		//makeResponseMsg(infoClient);
	}
	//sendReseponse(clientSocket);
	//infoClient.req.clearRequest();
	
	//(void)infoClient; // to be used

	// std::cout << " response to client : " << clientSocket << "\n";
	// long valWrite = write(clientSocket, resMsg.c_str(), resMsg.size());
	// if (valWrite == (long)resMsg.size())
	// 	std::cout << "SERVER RESPONSE SENT\n";
	// close(clientSocket);
}



std::string
Response::makeResponseGET(InfoClient &infoClient)
{
	std::string resMsg;
	char cwd[1024];
	getcwd(cwd, 1024);
	std::string cwdPath(cwd);
	if (infoClient.req.t_result.target == "/home" || infoClient.req.t_result.target == "/")
		resMsg = resMsgHeader(infoClient) + "\n" + resMsgBody(cwdPath + "/resource/static/index.html");
	else if (infoClient.req.t_result.target == "/server")
		resMsg = resMsgHeader(infoClient) + "\n" + resMsgBody(cwdPath + "/resource/static/server.html");
	else if (infoClient.req.t_result.target == "/submit")
		resMsg = resMsgHeader(infoClient) + "\n" + resMsgBody(cwdPath + "/resource/static/submit.html");
	else if (infoClient.req.t_result.target == "/upload")
		resMsg = resMsgHeader(infoClient) + "\n" + resMsgBody(cwdPath + "/resource/static/upload.html");
	else
		resMsg = makeResponseERR();
	return (resMsg);
}

std::string
Response::makeResponseERR()
{
	std::stringstream httpRes;
	std::string htmlMsg = "HTTP/1.1 400 Not Found\nContent-Type: text/html; charset=utf-8\nContent-Length: 200\n\n<h1>ERROR</h1> <h2>404 NOT FOUND</h2>";
	return (htmlMsg);
}

std::string
Response::resMsgHeader(InfoClient &infoClient)
{
	std::stringstream header;
	(void)infoClient;
	setStatusCode(200);
	setStatusMsg(_statusMap[200]);
	setConnection("keep-alive");
	if (infoClient.req.t_result.close == true)
		setConnection("close");
	setContentType("text/html");
	// setTransferEncoding("chunked");
	setContentLength(1000);

	header << getHttpVersion() << " " << getStatusCode() << " " << getStatusMsg() << CRLF;
	header << "Content-Type: " << getContentType() << "; charset=utf-8" << CRLF;
	header << "Connection: " << getConnection() << CRLF;
	header << "Date: " << timeStamp() << CRLF;
	header << "Server: " << "little webserver" << CRLF;
	// header << "Transfer-Encoding : chunked" << CRLF;
	header << "Content-Length: " << getContentLength() << CRLF;

	return (header.str());
}

std::string
Response::resMsgBody(std::string srcLocation)
{
	std::stringstream body;
	_fileFd = open(srcLocation.c_str(), O_RDONLY);
	if (_fileFd == -1)
		std::cerr << "	ERROR: file open error\n";
	int valRead = read(_fileFd, _fileBuff, sizeof(_fileBuff));
	if (valRead == -1)
		std::cerr << "	ERROR: read\n";
		std::cerr << "srcLocation : " << srcLocation << "\n";
		std::cerr << " valRead : " << valRead << " fileFd : " << _fileFd << "\n";
	if (_fileFd == -1 || valRead == -1)
	{
		body << "<h1>ERROR</h1><img src=\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAUAAAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO9TXL0Y4OHwAAAABJRU5ErkJggg==\" alt=\"Red dot\" />";
	}
	else
		body << _fileBuff;

	return body.str();
}

const char *
Response::getSendResult() const
{
	return (this->_result.c_str() + this->_sentBytes);
}

size_t
Response::getSendResultSize() const
{
	return (this->_totalBytes - this->_sentBytes);
}

size_t
Response::changePosition(int n)
{
	if (n > 0)
	{
		if (_sentBytes + n >= _totalBytes)
			_sentBytes = _totalBytes;
		else
			_sentBytes += n;
	}
	return (getSendResultSize());
}

void
Response::sendToClient(InfoClient &infoClient)
{

	size_t n = send(infoClient._clientSocket, getSendResult(), getSendResultSize(), 0);
	
	if (n < 0)
		status = sError;
	else if (changePosition(n) != 0)
		status = sSending;
	else
		status = sComplete;
}

void
Response::clearResult()
{
	_result.clear();
	_sentBytes = 0;
	_totalBytes = 0;
}

/*

 	1.해당 errorCode 에 대한 파일을 서버에서 가지고 있는 경우
	2. 없는 경우

*/
void
Response::makeErrorResponseMsg(InfoClient &infoClient, int errorCode)
{
	std::cout << "makeErrorResponseMsg "<< std::endl;
	this->status = rError;
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
	if (errorPath != "") //connection.cpp **if (_clientMap[currEvent->ident].status == InfoClient::fMaking)** 로직
	{
		infoClient.status = InfoClient::fMaking;
		std::string tmpPath = "configFiles/test.html";
		int fd;
		struct stat ss;
		if (stat(tmpPath.c_str(), &ss) == -1 || S_ISREG(ss.st_mode) != true ||
			(fd = open(tmpPath.c_str(), O_RDONLY)) == -1)
			std::cout << "errorPath failier" << std::endl;
		else
		{
			std::cout <<"file size = "<< ss.st_size << std::endl;
			std::cout << "fd = "<< fd<<std::endl;
			fcntl(fd, F_SETFL, O_NONBLOCK);
			infoClient.file.fd = fd;
		}
	}
	else //connection.cpp **else if (_clientMap[currEvent->ident].status == InfoClient::fComplete)** 로직
	{
		std::cout << "error message without file read\n";
	}
}

void
Response::makeResponseMsg(InfoClient &infoClient)
{
	int clientSocket = infoClient._clientSocket;
	std::cout << "makeResponseMsg "<< std::endl;

	std::string resMsg = "";
	if (infoClient.req.t_result.method == GET)
	{
		resMsg = makeResponseGET(infoClient);
		std::cout << "		------result msg------- : \n" << resMsg << "\n";

	}
	else if (infoClient.req.t_result.method == POST)
	{
		char cwd[1024];
		getcwd(cwd, 1024);
		std::string cwdPath(cwd);
		std::string execPath = "";
		std::string filePath = "";
		if (infoClient.req.t_result.target == "/www/cgi-bin/submit.py")
		{
			execPath = cwdPath + "/www/cgi-bin/submit.py";
			filePath = cwdPath + "/submit.html";
		}
		else if (infoClient.req.t_result.target == "/www/cgi-bin/upload.py")
		{
			std::cerr << "\n\nfor uploading \n\n";
			execPath = cwdPath + "/www/cgi-bin/upload.py";
			filePath = cwdPath + "/upload.html";
			std::string uploadPath = cwdPath + "/text.txt";

			std::cout << "\n\n" << uploadPath << "\n" << infoClient.req.t_result.body << "\n\n";
			int upFd = open(uploadPath.c_str(), O_WRONLY | O_CREAT, 0744);
			write(upFd, infoClient.req.t_result.body.c_str(), infoClient.req.t_result.body.size());
		}
		int pid = fork();
		waitpid(pid, NULL, 0);
		if (pid == 0)
		{
			int fd = open(filePath.c_str(), O_WRONLY | O_CREAT, 0744);
			dup2(fd, STDOUT_FILENO);
			char **args = new char *[sizeof(char *) * 4];
			args[0] = strdup("/usr/local/bin/python3");
			args[1] = strdup(execPath.c_str());
			args[2] = strdup(infoClient.req.t_result.body.c_str());
			args[3] = NULL;

			// char **env = (char **)malloc(sizeof(char *) * 4);
			// env[0] = strdup("REQUEST_METHOD=POST");
			// env[1] = strdup("SERVER_PROTOCOL=HTTP/1.1");
			// env[2] = strdup("PATH_INFO= ~~~ / ~~");
			// env[3] = NULL;
			execve("/usr/local/bin/python3", args, NULL);
			// execve(execPath.c_str(), args, NULL);
			perror("execute failed!!");
			exit(EXIT_FAILURE);
		}
		else
		{
			resMsg = resMsgHeader(infoClient) + "\n" + resMsgBody(filePath);
			std::cout << " response to client : " << clientSocket << "\n";
			long valWrite = write(clientSocket, resMsg.c_str(), resMsg.size());
			if (valWrite == (long)resMsg.size())
				std::cout << "SERVER RESPONSE SENT\n\n\n\n";
			// unlink(filePath.c_str());
			return ;
		}
	}
	else
	{
		resMsg = makeResponseERR();
	}

	(void)infoClient; // to be used

	std::cout << " response to client : " << clientSocket << "\n";
	long valWrite = write(clientSocket, resMsg.c_str(), resMsg.size());
	if (valWrite == (long)resMsg.size())
		std::cout << "SERVER RESPONSE SENT\n";
}

void
Response::initResponse(InfoClient &infoClient)
{
	setStatusCode(infoClient.req.t_result.status);
	setStatusMsg(_statusMap[getStatusCode()]);
	setDate();
	if (infoClient.req.t_result.close == true)
		setConnection("close");
	else
		setConnection("keep-alive");
	setContentType("text/html");
	setTransferEncoding("identity");
	setContentLength(infoClient.file.buffer.size());
	setBody(infoClient.file.buffer);
}

void
Response::startResponse(InfoClient &infoClient)
{
	initResponse(infoClient);
	_result += getHttpVersion() + " " + std::to_string(getStatusCode()) + CRLF;
	_result += "Connection : " + getConnection() + CRLF;
	_result += "Date : " + getDate() + CRLF;
	_result += "Server : " + getServer() + CRLF;
	_result += "Content-type : " + getContentType() + CRLF;
	_result += "Transfer-Encoding : " + getTransferEncoding() + CRLF;
	_result += "Content-Length : " + std::to_string(getContentLength()) + CRLF;
	_result += "\n";
	_result += infoClient.file.buffer;
}

bool
Response::cgiFinder(InfoClient &infoClient)
{
	std::string str = infoClient.req.t_result.path;
	//std::string str = "https://robodream.tistory.com/418.py";
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
	std::string str = infoClient.req.t_result.path;
	//std::string str = "https://robodream.tistory.com/redirectaa";
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



// ResponseManager::HandleGetErrorFailure(void) {
//   result_.status = 500;  // INTERNAL_SERVER_ERROR
//   is_keep_alive_ = false;
//   response_buffer_.content = LAST_ERROR_DOCUMENT;
//   io_status_ = SetIoComplete(IO_COMPLETE);
//   return IoFdPair();
// }

// ResponseManager::GenerateDefaultError(void) {
//   std::stringstream ss;
//   ss << result_.status << " " << g_status_map[result_.status];
//   response_buffer_.content = "<!DOCTYPE html><title>" + ss.str() +
//                              "</title><body><h1>" + ss.str() +
//                              "</h1></body></html>";
//   router_result_.error_path = "default_error.html";
//   io_status_ = SetIoComplete(IO_COMPLETE);
//   result_.ext = "html";
//   return IoFdPair();
// }

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


