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

void 		Request::updateStatus(int status, int pStatus) {
	_status = status;
	_pStatus = pStatus;
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
	if (std::count(mControl.begin(), mControl.end(),' ') != 2)
		return errorStatus("# Control Line Error <WhiteSpace>\n", 400, pError);
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return errorStatus("# Control Line Error <LENGTH>\n", 400, pError);

	if (mControl[0] == 'G')
		return parseControl(mControl, "GET");
	else if (mControl[0] == 'P')
		return parseControl(mControl, "POST");
	else if (mControl[0] == 'D')
		return parseControl(mControl, "DELETE");
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

void	Request::checkVersion()
{
	std::string version;

	version = getVersion();
	if (version.find(SP) != std::string::npos || version.size() != 8 || version.compare(0, 7, "HTTP/1.") != 0)
		updateStatus(400, pError); // Bad Request
	else if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		updateStatus(505, pError); // HTTP Version Not Supported
}



void	Request::parseHeader(size_t &prev)
{
	std::string	mOrig;
	std::string	mHeader;
	size_t		next;

	mOrig = getOrig();
	if (mOrig[prev + 2] == SP)
		return errorStatus("# Header Line Error <WhiteSpace>\n", 400, pError);
	next = mOrig.find("\r\n\r\n", prev + 2);
	if (next == std::string::npos)
		return errorStatus("# Header Line Error <Npos>\n", 400, pError);
	mHeader = mOrig.substr(prev + 2, next - prev);
	setHead(mHeader);
	checkHeader();
	prev = next + 4;
	if (_status == 200)
		_pStatus = pBody;
}

void	Request::checkHeader()
{
	std::string mHeader;
	size_t	pos_nl_prev;
	size_t	pos_nl_next;
	size_t	pos_colon;
	std::string fieldName;
	std::string fieldValue;

	mHeader = getHead();
	pos_nl_prev = -1;
	while (1) {
		pos_colon = mHeader.find(':', pos_nl_prev + 1);
		if (pos_colon == std::string::npos) {
			if (mHeader[pos_nl_prev + 1] != '\0')
				return errorStatus("# Header Line Error <Colon or \\r\\n>\n", 400, pError);
			break ;
		}
		if (mHeader.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1).find(SP) != std::string::npos)
			return errorStatus("# Header Line Error <Colon or \\r\\n>\n", 400, pError);
		// SP found between the header field_name and colon
		pos_nl_next = mHeader.find('\n', pos_colon + 1);
		if (pos_nl_next == std::string::npos || mHeader[pos_nl_next - 1] != '\r')
			return errorStatus("# Header Line Error <Colon or \\r\\n>\n", 400, pError);
		fieldName = mHeader.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1);
		fieldValue = mHeader.substr(pos_colon + 1, pos_nl_next - pos_colon - 2); // whitespace ?
//		header[fieldName] = fieldValue;
		pos_nl_prev = pos_nl_next;
	}
}


void	Request::parseBody(size_t &prev) // considerate content-size header
{
	_body = getOrig().substr(prev);
	_pStatus = pComplete;
}

void	Request::printRequest()
{
	if (_status == 200)
		std::cout <<
			  "[Request Line]" <<
			  "\nMethod:" << getMethod() <<
			  "\nTarget:" << getTarget() <<
			  "\nVersion:" << getVersion() <<
			  "\n[Header Info]\n" << getHead() <<
			  "\n[Body Info]\n" << getBody() <<
			  std::endl;
	else
		std::cout <<
			"[Status]:" <<
			_status <<
			"\n[Reason]:" <<
			_pStatus << // status <-> error map needed
			std::endl;
}

void	Request::errorStatus(std::string message, int status, int pStatus)
{
	updateStatus(status, pStatus);
	std::cout << message;
}