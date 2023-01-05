#include "Request.hpp"
#include <iostream>
#include <random>

void test(std::string mOrig);

int main()
{
	Request		request;
	std::string	mOrig;

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

	test(mOrig);

//	request.parseMessage(mOrig);
//	request.printRequest();

	return (0);
}

void test(std::string mOrig)
{
	Request		request;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(1, 10);
	std::string mInput;
	int 		len;

	while (!mOrig.empty())
	{
		len = dis(gen);
		mInput = mOrig.substr(0, len);
		mOrig.erase(0, len);
		std::cout << "Size:" << mInput.size() << " M:"<< mInput << "]\n";
		request.parseMessage(mInput);
		if (request.t_result.pStatus == Request::pComplete
			|| request.t_result.pStatus == Request::pError)
			break ;
	}

	request.printRequest();
}