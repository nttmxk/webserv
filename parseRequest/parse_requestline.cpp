#include "Request.hpp"

static int	checkWhitespace(std::string &mControl);
static int	checkVersion(std::string version);
static int	parseControl(Request &request, std::string &mControl, std::string method);

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

static int parseControl(Request &request, std::string &mControl, std::string method)
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
	request.setControl(mControl);
	return (0);
}

static int checkWhitespace(std::string &mControl)
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

static int checkVersion(std::string version)
{
	if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		return (-1);
	return (0);
}
