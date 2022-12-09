#include "Request.hpp"
#include <iostream>

// test function
int main()
{
	Request		request;
	std::string	mOrig;

	mOrig = "GET /hello.htm HTTP/1.1\r\n"
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
			"Host: www.tutorialspoint.com\r\n"
			"Accept-Language: en-us\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Connection: Keep-Alive\r\n"
			"\r\n"
			"";

	mOrig = "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
			"Host: www.tutorialspoint.com\r\n"
			"Content-Type: text/xml; charset=utf-8\r\n"
			"Content-Length: length\r\n"
			"Accept-Language: en-us\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Connection: Keep-Alive\r\n"
			"\r\n"
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
			"<string xmlns=\"http://clearforest.com/\">string</string>";
	request.setOrig(mOrig);

	if (parseMessage(request) == 0)
		printRequest(request);

	return (0);
}

int parseMessage(Request &request)
{
	size_t	pos;

	if (parseStartLine(request, pos) < 0)
		return (-1);
	if (parseHeader(request, pos) < 0)
		return (-1);
	if (parseBody(request, pos) < 0)
		return (-1);
	return (0);
}
