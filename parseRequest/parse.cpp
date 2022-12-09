#include "Request.hpp"
#include <iostream>

int		printError(std::string message);
int		checkWhitespace(std::string &mControl);
int		checkVersion(std::string version);

int		parseMessage(Request &request);
int		parseStartLine(Request &request, size_t &pos);
int		parseControl(Request &request, std::string &mControl, std::string method);
int		parseHeader(Request &request, size_t &prev);
int		parseBody(Request &request, size_t &prev);
void	printRequest(Request &request);

int main() // test function
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

	parseMessage(request);

	printRequest(request);

	return (0);
}

int	printError(std::string message)
{
	std::cout << message;
	return (-1);
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

int checkWhitespace(std::string &mControl)
{
	size_t	pos;
	int		count;

	pos = -1;
	count = 0;
	while (1){
		pos = mControl.find(' ', pos + 1);
		if (pos == std::string::npos)
			break ;
		count++;
	}
	if (count != 2)
		return (-1);
	return (0);
}

int checkVersion(std::string version)
{
	if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		return (-1);
	return (0);
}

int parseControl(Request &request, std::string &mControl, std::string method)
{
	size_t	len_method;
	size_t	pos;

	len_method = method.length();
	if (mControl.compare(0, len_method, method) != 0 || mControl[len_method] != ' ')
		return printError("# Control Line Error <" + method + ">\n");
	if (method == "GET") // ENUM?
		request.setMethod(0);
	else if (method == "POST")
		request.setMethod(1);
	else
		request.setMethod(2);
	pos = mControl.find(' ', len_method + 1);
	if (pos == std::string::npos)
		return printError("# Control Line Error <" + method + "/Npos>\n");
	request.setTarget(mControl.substr(len_method + 1, pos - len_method - 1));
	if (checkVersion(mControl.substr(pos + 1)) < 0)
		return printError("# Control Line Error <" + method + "/Version>\n");
	request.setVersion(mControl.substr(pos + 1));
	return (0);
}

int	parseStartLine(Request &request, size_t &pos)
{
	std::string	mOrig;
	std::string	mControl;

	mOrig = request.getOrig();
	pos = mOrig.find("\r\n", 0);
	if (pos == std::string::npos)
		return printError("# Control Line Error <Npos>\n");

	mControl = mOrig.substr(0, pos);
	if (checkWhitespace(mControl) < 0)
		return printError("# Control Line Error <WhiteSpace>\n");
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return printError("# Control Line Error <LENGTH>\n");

	if (mControl[0] == 'G')
		return parseControl(request, mControl, "GET");
	else if (mControl[0] == 'P')
		return parseControl(request, mControl, "POST");
	else if (mControl[0] == 'D')
		return parseControl(request, mControl, "DELETE");
	else
		return printError("# Control Line Error\n");
}

int	parseHeader(Request &request, size_t &prev)
{
	std::string	mOrig;
	std::string	mHeader;
	size_t		next;

	mOrig = request.getOrig();
	if (mOrig[prev + 2] == ' ')
		return printError("# Header Line Error <WhiteSpace>\n");
	next = mOrig.find("\r\n\r\n", prev + 2);
	if (next == std::string::npos)
		return printError("# Header Line Error <Npos>\n");

	mHeader = mOrig.substr(prev + 2, next - prev - 2);
	request.setHead(mHeader);
	prev = next + 4;
	return (0);
}

int	parseBody(Request &request, size_t &prev)
{
	std::string	mBody;

	mBody = request.getOrig().substr(prev);
	request.setBody(mBody);
	return (0);
}

void	printRequest(Request &request)
{
	std::cout <<
			  "[Request Line]" <<
			  "\nMethod:" << request.getMethod() <<
			  "\nTarget:" << request.getTarget() <<
			  "\nVersion:" << request.getVersion() <<
			  "\n[Header Info]\n" << request.getHead() <<
			  "\n[Body Info]\n" << request.getBody() <<
			  std::endl;
}