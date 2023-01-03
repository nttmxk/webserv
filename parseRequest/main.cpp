#include "Request.hpp"
#include <iostream>
#include <fstream>
// test function
int main()
{
	Request		request;
	std::string	mOrig;
//	std::ifstream file("request.txt");
//
//	if (file.fail())
//	{
//		return (1);
//	}
//	while (std::getline(file, mOrig))
//		;
//	file.close();

//	mOrig = "GET /hello.htm HTTP/1.1\r\n"
//			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
//			"Host: www.tutorialspoint.com\r\n"
//			"Accept-Language: en-us\r\n"
//			"Accept-Encoding: gzip, deflate\r\n"
//			"Connection: Keep-Alive\r\n"
//			"\r\n"
//			"";

	mOrig = "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
			"Host: www.tutorialspoint.com\r\n"
			"Content-Type: text/xml; charset=utf-8\r\n"
			"Content-Length: 95           \r\n"
			"Accept-Language: en-us\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Connection:          Keep-Alive           \r\n"
			"\r\n"
			"<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
			"<string xmlns=\"http://clearforest.com/\">string</string>";

	request.setOrig(mOrig);
	request.parseMessage();

	request.printRequest();

	return (0);
}
