#include "Request.hpp"

Request::Request() {
	_pStatus = pRequest;
	_status = 200;
}

Request::~Request() {}

Request::Request(const Request &orig) {}

Request& Request::operator=(const Request &orig) {
	return (*this);
}

std::string Request::getOrig() { return (_orig); }
std::string Request::getHead() { return (_head); }
std::string Request::getBody() { return (_body); }
std::string	Request::getTarget() { return (_target); }
std::string	Request::getVersion() { return (_version); }
int 		Request::getMethod() { return (_method); }
int 		Request::getStatus() { return (_status); }

void 		Request::setOrig(std::string &orig) {
	_orig = orig;
}

void		Request::setHead(std::string &head) {
	_head = head;
}

void		Request::setBody(std::string &body) {
	_body = body;
}

void 		Request::setTarget(const std::string &target) {
	_target = target;
}

void		Request::setVersion(const std::string &version) {
	_version = version;
}

void 		Request::setMethod(int method) {
	_method = method;
}

void 		Request::updateStatus(int status) {
	_status = status;
}

void	Request::parseMessage()
{
	size_t	pos;

	if (_pStatus == pRequest)
		parseStartLine(pos);
	if (_pStatus == pHeader)
		parseHeader(pos);
	if (_pStatus == pBody)
		parseBody(pos);
}

void	Request::parseStartLine(size_t &pos)
{
	std::string	mControl;

	pos = _orig.find(CRLF, 0);
	if (pos == std::string::npos)
		return errorStatus("# Control Line Error <Npos>\n", 400, pError);

	mControl = _orig.substr(0, pos);
	if (checkWhitespace(mControl) < 0)
		return errorStatus("# Control Line Error <WhiteSpace>\n", 400, pError);
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return errorStatus("# Control Line Error <LENGTH>\n", 400, pError);

	if (mControl[0] == 'G')
		return parseControl(mControl, "GET");
	else if (mControl[0] == 'P')
		return parseControl(request, mControl, "POST");
	else if (mControl[0] == 'D')
		return parseControl(request, mControl, "DELETE");
	else
		return errorStatus("# Control Line Error\n", 501, pError); // Not Implemented
}

void Request::parseControl(std::string &mControl, std::string method)
{
	size_t	len_method;
	size_t	pos;

	len_method = method.length();
	if (mControl.compare(0, len_method, method) != 0 || mControl[len_method] != SP)
		return errorStatus("# Control Line Error <" + method + ">\n", 501, pError); // Not Implemented
	if (method == "GET")
		_method = GET;
	else if (method == "POST")
		_method = POST;
	else
		_method = DELETE;
	pos = mControl.find(SP, len_method + 1);
	if (pos == std::string::npos)
		return errorStatus("# Control Line Error <" + method + "/Npos>\n", 400, pError);

	_target = mControl.substr(len_method + 1, pos - len_method - 1); // Status 414 need to be checked
	_version = mControl.substr(pos + 1);
	checkVersion();
	if (_status == 200)
		_pStatus = pHeader;
}

int Request::checkWhitespace(std::string &mControl)
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

void	Request::checkVersion()
{
	std::string version;

	version = getVersion();
	if (version.find(SP) != std::string::npos || version.size() != 8 || version.compare(0, 7, "HTTP/1.") != 0)
		updateStatus(400, pError); // Bad Request
	else if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		updateStatus(505, pError); // HTTP Version Not Supported
}



void	Request::parseBody(size_t &prev)
{
	_body = getOrig().substr(prev);
}
