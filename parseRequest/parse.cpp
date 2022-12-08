#include "Request.hpp"
#include <iostream>

int	printError(std::string message) // add to message "<Reason>" for debug purpose
{
	std::cout << message;
	return (-1);
}

//int parseMessage(Request &request)
//{
//	if (parseRequest(request) < 0)
//		return (-1);
//	if (parseHeader(request) < 0)
//		return (-1);
//	if (parseBody(request) < 0)
//		return (-1);
//	return (0);
//}

int checkVersion(std::string version)
{
	if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		return (-1);
	return (0);
}

int parseControl(Request &request, std::string &mControl, std::string method)
{
	size_t	len_method;
	int pos;

	len_method = method.length();
	if (mControl.compare(0, len_method, method) != 0 || mControl[len_method] != ' ')
		return printError("# Control Line ERROR <" + method + ">\n");
	request.setMethod(0); // ENUM?
	pos = mControl.find(' ', len_method + 1);
	if (pos == std::string::npos)
		return printError("# Control Line ERROR <" + method + "/Npos>\n");
	request.setTarget(mControl.substr(len_method + 1, pos - len_method - 1));
	if (checkVersion(mControl.substr(pos + 1)) < 0)
		return printError("# Control Line ERROR <" + method + "/Version>\n");
	request.setVersion(mControl.substr(pos + 1));
	/////////// testing
	std::cout << "Method:" << method <<
	"\nTarget:" << request.getTarget() <<
	"\nVersion:" << request.getVersion() << '\n';
	///////////
	return (0);
}

int	parseRequest(Request &request)
{
	std::string	mOrig, mControl;
	size_t		pos;

//	mOrig = request.getOrig();
	mOrig = "GET / HTTP/1.0\r\n";     ////////// testing
	pos = mOrig.find("\r\n", 0); // What if it ends with "\r\r\n"
	if (pos == std::string::npos)
		return printError("# Control Line ERROR <Npos>\n");

	mControl = mOrig.substr(0, pos);
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return printError("# Control Line ERROR <LENGTH>\n");

	if (mControl[0] == 'G')
		return (parseControl(request, mControl, "GET"));
	else if (mControl[0] == 'P')
		return (parseControl(request, mControl, "POST"));
	else if (mControl[0] == 'D')
		return (parseControl(request, mControl, "DELETE"));
	else
		return printError("# Control Line ERROR\n");
}

int main() // test function
{
	Request request;

	parseRequest(request);

	return (0);
}