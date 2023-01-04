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

	mOrig = "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
			"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
			"Host: www.tutorialspoint.com\r\n"
			"Content-Type: text/xml; charset=utf-8\r\n"
			"transfer-encodinG: chunked           \r\n"
			"Accept-Language: en-us\r\n"
			"Accept-Encoding: gzip, deflate\r\n"
			"Connection:          Keep-Alive           \r\n"
			"\r\n"
			"3\r\n"
			"abc\r\n"
			"4\r\n"
			"dfer\r\n"
			"A\r\n"
			"0123456789\r\n"
			"0\r\n";

//	request.parseMessage(mOrig);
//	request.printRequest();
//	mOrig = "3\r\n";
//	mOrig += "abc\r\n";
//	request.parseMessage(mOrig);
//	request.printRequest();
//	mOrig = "4\r\n";
//	mOrig += "dfer\r\n";
//	mOrig += "A\r\n";
//	request.parseMessage(mOrig);
//	request.printRequest();
//	mOrig = "0123456789\r\n";
//	mOrig += "0\r\n";
//	request.parseMessage(mOrig);
//	request.printRequest();

	request.parseMessage(mOrig);
	while (request.t_result.pStatus != Request::pComplete
		&& request.t_result.pStatus != Request::pError)
		request.parseMessage("");

	request.printRequest();

	return (0);
}
