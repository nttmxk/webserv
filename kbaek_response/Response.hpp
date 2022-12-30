#include "Cgi.hpp"
#include <iostream>
#include <string>
#include <map>

class Response 
{
public:
	Response();
	~Response();

	std::string getResponseMessage(void);
	std::string getResult(void);
	bool sendFromWbuf(int fd);

  struct Result {
    std::string status;
    std::string headers;
    // int status;
	std::string body;
    std::string location; 

  };

private:
	std::string message;


};

Response::Response(/* args */)
{
}

Response::~Response()
{
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