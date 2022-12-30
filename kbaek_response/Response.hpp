#include "Cgi.hpp"
#include <iostream>
#include <string>
#include <map>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

class Response 
{
public:
	Response();
	~Response();

	std::string getResponseMessage(void);
	std::string getResult(void);
	bool sendFromWbuf(int fd);


	void getMethod();
	void postMethod();
	int openFile(const char* path);

  struct Result {
    std::string status;
    std::string headers;
    // int status;
	std::string body;
    std::string location; 

  };

private:
	std::string message;
	Result result;

};

Response::Response(/* args */)
{
}

Response::~Response()
{
}

void
getMethod()
{
	openFile();
	initResponse();

}

void
postMethod()
{

}

//리소스에 접근했을 때 실제 파일이 있는지 찾아서 read 를 통해 확인하는 함수
int
Response::openFile(const char* path) {
  errno = 0;
  int fd = open(path, O_RDONLY, 0644);
  if (fd == -1) {
    if(errno == EACCES)
      result.status = 403;  // FORBIDDEN 
	else if (errno == ENOENT)
      result.status = 404;  // PAGE NOT FOUND
    else if (errno == EMFILE)
      result.status = 503;  // SERVICE UNAVAILABLE
    else
      result.status = 500;  // INTERNAL SERVER ERROR}
  } else
    fcntl(fd, F_SETFL, O_NONBLOCK);
  return fd;
}

std::string
Response::getResult(void)
{
	std::map<std::string, std::string>::const_iterator it = this->m_headers.begin();

	message = m_protocol + " " + ft::to_string(m_status_code) + " " + m_status_description + "\r\n";
	for (; it != this->m_headers.end(); ++it)
		message += it->first + ": " + it->second + "\r\n";
	if (m_connection_type == CLOSE || m_status_code < 200 || m_status_code > 299)
		message += "Connection: close\r\n";
	else
		message += "Connection: Keep-Alive\r\n";
	if (m_transfer_type == CHUNKED) {
		message += "Transfer-Encoding: chunked\r\n\r\n";
		int size = this->m_content.size();
		int count;
		std::string data = m_content;
		int added = 0;
		while (size > 0)
		{
			if (size > BUFFER_SIZE)
				count = BUFFER_SIZE;
			else
				count = size;
			message += ft::itos(ft::to_string(count), 10, 16) + "\r\n";
			message += data.substr(added, count) + "\r\n";
			size -= count;
			added += count;
		}
		data.clear();
		message += "0\r\n\r\n";
	}
	else
	{
		message += "\r\n";
		message += this->m_content;
	}
	return (message);
}

bool
Response::sendFromWbuf(int fd)
{
	int count = m_wbuf_data_size - m_send_data_size;
	if (count > BUFFER_SIZE)
		count = BUFFER_SIZE;
	count = send(fd, m_wbuf.c_str() + m_send_data_size, count, 0);
	if (count == 0 || count == -1) 
		throw (Server::IOError((("IO error detected to send response message to client ") + ft::to_string(fd)).c_str()));
	m_send_data_size += count;
	return (true);
}