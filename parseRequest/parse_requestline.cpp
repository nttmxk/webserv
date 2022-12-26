#include "Request.hpp"

static int	checkWhitespace(std::string &mControl);
static int	checkVersion(std::string version);
static int	parseControl(Request &request, std::string &mControl, std::string method);

int	parseStartLine(Request &request, size_t &pos)
{
	std::string	mOrig;
	std::string	mControl;

	mOrig = request.getOrig();
	pos = mOrig.find(CRLF, 0);
	if (pos == std::string::npos)
		return errorStatus("# Control Line Error <Npos>\n", 400);

	mControl = mOrig.substr(0, pos);
	if (checkWhitespace(mControl) < 0)
		return errorStatus("# Control Line Error <WhiteSpace>\n", 400);
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return errorStatus("# Control Line Error <LENGTH>\n", 400);

	if (mControl[0] == 'G')
		return parseControl(request, mControl, "GET");
	else if (mControl[0] == 'P')
		return parseControl(request, mControl, "POST");
	else if (mControl[0] == 'D')
		return parseControl(request, mControl, "DELETE");
	else
		return errorStatus("# Control Line Error\n", 501); // Not Implemented
}

static int parseControl(Request &request, std::string &mControl, std::string method)
{
	size_t	len_method;
	size_t	pos;

	len_method = method.length();
	if (mControl.compare(0, len_method, method) != 0 || mControl[len_method] != SP)
		return errorStatus("# Control Line Error <" + method + ">\n", 501); // Not Implemented
	if (method == "GET")
		request.setMethod(GET);
	else if (method == "POST")
		request.setMethod(POST);
	else
		request.setMethod(DELETE);
	pos = mControl.find(SP, len_method + 1);
	if (pos == std::string::npos)
		return errorStatus("# Control Line Error <" + method + "/Npos>\n", 400);
	request.setTarget(mControl.substr(len_method + 1, pos - len_method - 1)); // Status 414 need to be checked
	if (checkVersion(mControl.substr(pos + 1)) < 0)
		return printError("# Control Line Error <" + method + "/Version>\n");
	request.setVersion(mControl.substr(pos + 1));
	return (0);
}

static int checkWhitespace(std::string &mControl)
{
	size_t	pos;
	int		count;

	pos = -1;
	count = 0;
	while (1) {
		pos = mControl.find(SP, pos + 1);
		if (pos == std::string::npos)
			break ;
		count++;
	}
	if (count != 2)
		return (-1);
	return (0);
}

static int checkVersion(std::string version)
{
	if (version.find(SP) != std::string::npos || version.size() != 8 || version.compare(0, 7, "HTTP/1.") != 0)
	{
		updateStatus(400); // Bad Request
		return (-1);
	}
	if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
	{
		updateStatus(505); // HTTP Version Not Supported
		return (-1);
	}
	return (0);
}
