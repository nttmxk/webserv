#include "Request.hpp"

std::string Request::getOrig() { return (_orig); }
std::string Request::getHead() { return (_head); }
std::string	Request::getTarget() { return (_target); }
std::string	Request::getVersion() { return (_version); }

/**
 * 	@brief	Set original message from client to Request class
 * 	@param	original message from client
 */
void 		Request::setOrig(std::string &orig) {
	_orig = orig;
}

/**
 *	@brief	Update status code and parsing status
 *	@param	status code, parsing status to update
 */
void 		Request::updateStatus(int status, int pStatus) {
	t_result.status = status;
	_pStatus = pStatus;
}

/**
 * 	@brief	Parsing original message from client
 */
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

/**
 *	@brief	Parsing request line
 *	@param
 */
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

/**
 * 	@brief	Parsing control line
 * 	@param	control line message, type of the method
 */
void Request::parseControl(std::string &mControl, std::string method)
{
	size_t	len_method;
	size_t	pos;

	len_method = method.length();
	if (mControl.compare(0, len_method, method) != 0 || mControl[len_method] != SP)
		return errorStatus("# Control Line Error <" + method + ">\n", 501, pError); // Not Implemented
	if (method == "GET")
		t_result.method = GET;
	else if (method == "POST")
		t_result.method = POST;
	else
		t_result.method = DELETE;
	pos = mControl.find(SP, len_method + 1);
	if (pos == std::string::npos)
		return errorStatus("# Control Line Error <" + method + "/Npos>\n", 400, pError);

	_target = mControl.substr(len_method + 1, pos - len_method - 1); // Status 414 need to be checked
//	Uri uriParser;
//	uriParser.parseTarget(_target);

	_version = mControl.substr(pos + 1);
	checkVersion();

	if (t_result.status == 200)
		_pStatus = pHeader;
}

/**
 * 	@brief	checking http version
 */
void	Request::checkVersion()
{
	std::string version;

	version = getVersion();
	if (version.find(SP) != std::string::npos || version.size() != 8 || version.compare(0, 7, "HTTP/1.") != 0)
		return updateStatus(400, pError); // Bad Request
	if (version.compare("HTTP/1.1") != 0 && version.compare("HTTP/1.0") != 0)
		return updateStatus(505, pError); // HTTP Version Not Supported
}

/**
 * 	@brief	parsing header
 * 	@param	starting position of the header message
 */
void	Request::parseHeader(size_t &prev)
{
	std::string	mOrig;
	std::string	mHeader;
	size_t		next;

	mOrig = getOrig();
	if (mOrig[prev + 2] == SP)
		return errorStatus("# Header Line Error <WhiteSpace>\n", 400, pError);
	next = mOrig.find(CRLF CRLF, prev + 2);
	if (next == std::string::npos)
		return errorStatus("# Header Line Error <Npos>\n", 400, pError);
	mHeader = mOrig.substr(prev + 2, next - prev);
	_head = mHeader;
	tokenizeHeader();
	prev = next + 4;
	if (t_result.status == 200)
		_pStatus = pBody;
}

/**
 * 	@brief	Tokenizing header field name and header field value
 */
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
		std::transform(fieldValue.begin(), fieldValue.end(), fieldValue.begin(), ::tolower);
		std::transform(fieldName.begin(), fieldName.end(), fieldName.begin(), ::tolower);
		t_result.header[fieldName] = fieldValue;
		pos_nl_prev = pos_nl_next;
	}
	verifyHeader();
}

/**
 * 	@brief	Determining if the character is optional whitespace or not
 * 	@param	the character to determine
 */
bool	Request::isOWS(int c)
{
	if (c == SP || c == HTAB)
		return true;
	return false;
}

/**
 * 	@brief	Verifying host, content-length, connection header
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

/**
 * 	@brief	checking host header
 */
void 	Request::checkHost()
{
	std::map<std::string, std::string>::iterator it;

	it = t_result.header.find("host");
	if (it == t_result.header.end())
		return errorStatus("Host cannot be empty", 400, pError);
//	if (verifyHost())
	t_result.host = it->second;
}

/**
 *	@brief	checking content-length header and chunked option
 */
void 	Request::checkBodyLength()
{
	std::map<std::string, std::string>::iterator it;

	it = t_result.header.find("content-length");
	if (it == t_result.header.end())
		_bodyLength = -1;
	else
	{
		if (it->second.find_first_not_of(DIGIT) != std::string::npos)
			return errorStatus("CL should only include DIGIT", 400, pError);
		std::stringstream ss(it->second);
		ss >> _bodyLength; // client_max_body_size check
	}

	it = t_result.header.find("transfer-encoding");
	if (it != t_result.header.end())
	{
		if (_bodyLength != -1)
			return errorStatus("CL and TE are both exist", 400, pError);
		if (it->second.compare("chunked") == 0)
			_chunked = true;
		else
			; // what to do with deflate, compress, gzip etc...
	}
}

/**
 * 	@brief	checking connection header
 */
void 	Request::checkConnection()
{
	std::map<std::string, std::string>::iterator it;

	it = t_result.header.find("connection");
	if (it == t_result.header.end())
		return ;
	if (it->second.compare("close") == true)
		t_result.close = true;
}

/**
 * 	@brief	parsing content message
 * 	@param	starting position of the content message
 */
void	Request::parseBody(size_t &prev) // considerate content-size header
{
	/*
	 * only TE -> _bodyLength = -1 and _chunked = true -> _bodyLength != -1 and _chunked = true
	 * both No -> _bodyLength = -1 and _chunked = false
	 * only CL -> _bodyLength >= 0 and _chunked = false
	 */
	if (_chunked)
		return parseChunked();
	if (_bodyLength == -1)
	{
		_pStatus = pComplete;
		return ;
	}
	t_result.body.append(getOrig().substr(prev));
	if (t_result.body.size() >= _bodyLength)
	{
		_pStatus = pComplete;
		if (t_result.body.size() != _bodyLength)
			t_result.body.erase(_bodyLength);
	}
	else
		; // content length doesn't match.... need more message or 400 bad request + close connection
}

/**
 * 	@brief	parsing chunked content
 * 	@param	starting position of the content message
 */
void 	Request::parseChunked()
{
	std::string			message;
	size_t				pos;
	size_t				pos_next;
	int 				temp_bodyLength;

	if (_bodyLength == -1) // first time here, no more message
	{
		_bodyLength = 0;
		return ;
	}
	message = getOrig();
	pos = message.find(CRLF);
	if (pos == std::string::npos) // chunked size max also needed
		return errorStatus("Chunk message first CRLF error", 400, pError);

	std::stringstream ss(message.substr(0, pos));
	ss >> std::hex >> temp_bodyLength;
	if (temp_bodyLength == 0) // receiving is done
	{
		if (pos != 1)
			return errorStatus("Chunk message 0 error", 400, pError);
		_pStatus = pComplete;
		return ;
	}
	// temp_bodyLength > 0 check?
	// should I check with message.compare(pos + 2 + temp_bodyLength, 2, CRLF) ?
	pos_next = message.find(CRLF, pos + 2);
	if (pos_next == std::string::npos)
		return errorStatus("Chunk message second CRLF error", 400, pError);

	if (pos_next - pos - 2 != temp_bodyLength)
		return errorStatus("Chunk message length doesn't match", 400, pError);
	_bodyLength += temp_bodyLength; // client_max_body_size check
	t_result.body.append(message.substr(pos + 2, temp_bodyLength));
	// if there is more message after second CRLF, abandon that message for now
}

/**
 * 	@brief	printing request class
 */
void	Request::printRequest()
{
	if (t_result.status == 200)
		std::cout <<
			  "[Request Line]" <<
			  "\nTarget:" << getTarget() << // soon to be deleted
			  "\nVersion:" << getVersion() <<
			  std::endl;
	else
		std::cout <<
			"\n[Status]:" <<
			t_result.status <<
			"\n[Reason]:" <<
			_pStatus << // status <-> error map needed
			std::endl;
	std::cout << "[Result Info]" <<
				"\nMethod:" << t_result.method <<
				"\nVersion:" << t_result.version <<
				"\nStatus:" << t_result.status <<
				"\nClose:" << t_result.close <<
				"\nBody:" << t_result.body <<
				"\nPath:" << t_result.path <<
				"\nQuery:" << t_result.query <<
				"\nHost:" << t_result.host
				;
	std::cout << "[Header Info]\n";
	for (std::map<std::string, std::string>::iterator it = t_result.header.begin(); it != t_result.header.end() ; ++it) {
		std::cout << it->first << ":[" << it->second << "]\n";
	}
}

/**
 * 	@brief	set error status and print error message
 * 	@param	message to print, status code, parsing status
 */
void	Request::errorStatus(std::string message, int status, int pStatus)
{
	updateStatus(status, pStatus);
	std::cout << message;
}

Request::Request(): _pStatus(pRequest), _bodyLength(0), _chunked(false) {
	t_result.status = 200;
}

Request::~Request() {}

Request::Request(const Request &orig) {
	*this = orig;
}

Request& Request::operator=(const Request &orig) {
	_orig = orig._orig;
	_head = orig._head;
	_target = orig._target;
	_version = orig._version;
	_pStatus = orig._pStatus;
	t_result = orig.t_result;
	return (*this);
}