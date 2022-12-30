#include "Request.hpp"

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

void 		Request::updateStatus(int status, int pStatus) {
	_status = status;
	_pStatus = pStatus;
}

void	Request::parseMessage()
{
	size_t	pos;

	if (_pStatus == pRequest)
		parseRequestLine(pos);
	if (_pStatus == pHeader)
		parseHeader(pos);
	if (_pStatus == pBody)
		parseBody(pos);
}

void	Request::parseRequestLine(size_t &pos)
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

	_target = mControl.substr(len_method + 1, pos - len_method - 1); // Status 414 need to be checkedv
//	Uri uriParser;
//	uriParser.parseTarget(_target);

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
		return updateStatus(400, pError); // Bad Request
	if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		return updateStatus(505, pError); // HTTP Version Not Supported
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
	_head = mHeader;
	tokenizeHeader();
	prev = next + 4;
	if (_status == 200)
		_pStatus = pBody;
}

void	Request::tokenizeHeader()
{
	std::string mHeader;
	size_t	pos_nl_prev;
	size_t	pos_nl_next;
	size_t	pos_start;
	size_t	pos_end;
	size_t	pos_colon;
	std::string fieldName;
	std::string fieldValue;

	mHeader = getHead();
	pos_nl_prev = -1;
	while (1) {
		pos_colon = mHeader.find(':', pos_nl_prev + 1);
		if (pos_colon == std::string::npos) {
			if (mHeader[pos_nl_prev + 1] != '\0')
				return errorStatus("# Header Line Error <Colon>\n", 400, pError);
			break ;
		}
		if (mHeader.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1).find(SP) != std::string::npos)
			return errorStatus("# Header Line Error <SP in field_name>\n", 400, pError);
		pos_nl_next = mHeader.find('\n', pos_colon + 1);
		if (pos_nl_next == std::string::npos || mHeader[pos_nl_next - 1] != '\r')
			return errorStatus("# Header Line Error <\\r\\n>\n", 400, pError);
		fieldName = mHeader.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1);
		pos_start = pos_colon + 1;
		pos_end = pos_nl_next - 2;
		while (isOWS(mHeader[pos_start]))
			++pos_start;
		while (isOWS(mHeader[pos_end]))
			--pos_end;
		fieldValue = mHeader.substr(pos_start, pos_end - pos_start + 1);
		header[fieldName] = fieldValue;
		pos_nl_prev = pos_nl_next;
	}
	verifyHeader();
}

bool	Request::isOWS(int c)
{
	if (c == SP || c == HTAB)
		return true;
	return false;
}

/*
 * host
 * connection
 * content-length
 * transfer-encoding
 * content-type || Cookie
 */
void 	Request::verifyHeader()
{
	if (_pStatus == pHeader)
		checkHost();
	if (_pStatus == pHeader)
		checkBodyLength();
	if (_pStatus == pHeader)
		checkConnection();
}

void 	Request::checkHost()
{
	std::map<std::string, std::string>::iterator it;

	it = header.find("Host");
	if (it == header.end())
		return errorStatus("Host cannot be empty", 400, pError);
//	if (verifyHost())
	t_result.host = it->second;
}

void 	Request::checkBodyLength()
{
	std::map<std::string, std::string>::iterator it;

	it = header.find("Content-Length"); // Upper-case?
	if (it == header.end())
		_bodyLength = -1;
	else
	{
		if (it->second.find_first_not_of(DIGIT) != std::string::npos)
			return errorStatus("CL should only include DIGIT", 400, pError);
		std::stringstream ss(it->second);
		ss >> _bodyLength; // client_max_body_size check
	}
	it = header.find("Transfer-Encoding");
	if (it != header.end())
	{
		if (_bodyLength != -1)
			return errorStatus("CL and TE are both exist", 400, pError);
		if (it->second.compare("chunked") == 0)
			_chunked = true;
		else
			; // what to do with deflate, compress, gzip etc...
	}
}

void 	Request::checkConnection()
{
	std::map<std::string, std::string>::iterator it;

	it = header.find("Connection");
	if (it == header.end())
		return ;
	if (it->second.compare("close"))
		t_result.close = true;
}

void	Request::parseBody(size_t &prev) // considerate content-size header
{
	if (_bodyLength == -1)
	{
		_pStatus = pComplete;
		return ;
	}
	if (_chunked)
		return parseChunked(prev);
	_body = getOrig().substr(prev); // append?
	if (_body.size() >= _bodyLength)
	{
		if (_body.size() == _bodyLength)
			_pStatus = pComplete;
		else
			; // huh?
	}
	else
		; // huh?
}

void 	Request::parseChunked(size_t &prev)
{;}

void	Request::printRequest()
{
	if (_status == 200)
		std::cout <<
			  "[Request Line]" <<
			  "\nMethod:" << getMethod() <<
			  "\nTarget:" << getTarget() <<
			  "\nVersion:" << getVersion() <<
//			  "\n[Header Info]\n" << getHead() <<
			  "\n[Body Info]\n" << getBody() <<
			  std::endl;
	else
		std::cout <<
			"[Status]:" <<
			_status <<
			"\n[Reason]:" <<
			_pStatus << // status <-> error map needed
			std::endl;
	std::cout << "[HEADER MAP]\n";
	for (std::map<std::string, std::string>::iterator it = header.begin(); it != header.end() ; ++it) {
		std::cout << it->first << ":[" << it->second << "]\n";
	}
}

void	Request::errorStatus(std::string message, int status, int pStatus)
{
	updateStatus(status, pStatus);
	std::cout << message;
}

Request::Request(): _status(200), _pStatus(pRequest), _bodyLength(0), _chunked(false) {}

Request::~Request() {}

Request::Request(const Request &orig) {
	*this = orig;
}

Request& Request::operator=(const Request &orig) {
	_orig = orig._orig;
	_head = orig._head;
	_body = orig._body;
	_target = orig._target;
	_version = orig._version;
	_status = orig._status;
	_pStatus = orig._pStatus;
	_method = orig._method;
	header = orig.header;
	return (*this);
}