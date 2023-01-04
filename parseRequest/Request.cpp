#include "Request.hpp"

std::string Request::getHead() { return (_head); }
std::string	Request::getTarget() { return (_target); }
std::string	Request::getVersion() { return (_version); }


/**
 *	@brief	Set request class to empty status
 *	@when	After send a response to client
 */
void 		Request::clearRequest()
{
	_orig.clear();
	_bodyLength = 0;
	_chunked = false;
	t_result.status = 200;
	t_result.pStatus = pRequest;
	t_result.body.clear();
	t_result.header.clear();
}

/**
 *	@brief	Update status code and parsing status
 *	@param	status code, parsing status to update
 */
void 		Request::updateStatus(int status, int pStatus) {
	t_result.status = status;
	t_result.pStatus = pStatus;
}

/**
 * 	@brief	Parsing original message from client
 * 	@param	message from client
 */
void	Request::parseMessage(std::string message)
{
	size_t	pos;

	_orig += message;
	if (t_result.pStatus == pRequest)
		parseRequestLine();
	if (t_result.pStatus == pHeader)
		parseHeader();
	if (t_result.pStatus == pBody)
		parseBody();
}

/**
 *	@brief	Parsing request line
 *			1. method
 *			2. target
 *			3. http version
 */
void	Request::parseRequestLine()
{
	std::string	mControl;
	size_t		pos;

	pos = _orig.find(CRLF, 0);
	if (pos == std::string::npos)
	{
//		if (IF TOO LONG)
//			return errorStatus("# Control Line Error <Npos>\n", 400, pError);
		return ;
	}

	mControl = _orig.substr(0, pos);
	_orig.erase(0, pos + 2);

	if (std::count(mControl.begin(), mControl.end(),' ') != 2)
		return errorStatus("# Control Line Error <WhiteSpace>\n", 400, pError);
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return errorStatus("# Control Line Error <LENGTH>\n", 400, pError);

	checkMethod(mControl);

	pos = mControl.find(SP);
	_target = mControl.substr(0, pos);
	checkTarget();

	_version = mControl.erase(0, pos + 1);
	checkVersion();

	if (t_result.status == 200)
		t_result.pStatus = pHeader;
}

/**
 *	@brief	Checking method
 *	@param	control line message
 */
void Request::checkMethod(std::string &mControl)
{
	if (mControl[0] == 'G')
		return validateMethod(mControl, "GET");
	else if (mControl[0] == 'P')
		return validateMethod(mControl, "POST");
	else if (mControl[0] == 'D')
		return validateMethod(mControl, "DELETE");
	return errorStatus("# Control Line Error\n", 501, pError); // Not Implemented
}

/**
 * 	@brief	Parsing control line
 * 	@param	control line message, type of the method
 */
void Request::validateMethod(std::string &mControl, std::string method)
{
	size_t	len_method;

	len_method = method.length();
	if (mControl.compare(0, len_method, method) != 0 || mControl[len_method] != SP)
		return errorStatus("# Control Line Error <" + method + ">\n", 501, pError); // Not Implemented
	if (method == "GET")
		t_result.method = GET;
	else if (method == "POST")
		t_result.method = POST;
	else
		t_result.method = DELETE;
	mControl.erase(0, len_method + 1);
}

/**
 *	@brief	Checking target
 */
void Request::checkTarget()
{
	//	Uri uriParser;
	//	uriParser.parseTarget(_target);
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
 */
void	Request::parseHeader()
{
	size_t		pos;

	pos = _orig.find(CRLF CRLF);
	if (pos == std::string::npos)
	{
//		if (SOMETHING)
//			return errorStatus("# Header Line Error <Npos>\n", 400, pError);
		return ;
	}
	_head = _orig.substr(0, pos + 2);
	_orig.erase(0, pos + 4);
	tokenizeHeader();
	verifyHeader();
	if (t_result.status == 200)
		t_result.pStatus = pBody;
}

/**
 * 	@brief	Tokenizing header field name and header field value
 */
void	Request::tokenizeHeader()
{
	size_t		pos_nl_prev;
	size_t		pos_nl_next;
	size_t		pos_start;
	size_t		pos_end;
	size_t		pos_colon;
	std::string	fieldName;
	std::string	fieldValue;

	pos_nl_prev = -1;
	while (1) {
		pos_colon = _head.find(':', pos_nl_prev + 1);
		if (pos_colon == std::string::npos) {
			if (_head[pos_nl_prev + 1] != '\0')
				return errorStatus("# Header Line Error <Colon>\n", 400, pError);
			break ;
		}
		if (_head.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1).find(SP) != std::string::npos)
			return errorStatus("# Header Line Error <SP in field_name>\n", 400, pError);
		pos_nl_next = _head.find('\n', pos_colon + 1);
		if (pos_nl_next == std::string::npos || _head[pos_nl_next - 1] != '\r')
			return errorStatus("# Header Line Error <\\r\\n>\n", 400, pError);
		fieldName = _head.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1);
		pos_start = pos_colon + 1;
		pos_end = pos_nl_next - 2;
		while (isOWS(_head[pos_start]))
			++pos_start;
		while (isOWS(_head[pos_end]))
			--pos_end;
		fieldValue = _head.substr(pos_start, pos_end - pos_start + 1);
		std::transform(fieldValue.begin(), fieldValue.end(), fieldValue.begin(), ::tolower);
		std::transform(fieldName.begin(), fieldName.end(), fieldName.begin(), ::tolower);
		t_result.header[fieldName] = fieldValue;
		pos_nl_prev = pos_nl_next;
	}
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
	if (t_result.pStatus == pHeader)
		checkHost();
	if (t_result.pStatus == pHeader)
		checkBodyLength();
	if (t_result.pStatus == pHeader)
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
		return errorStatus("Host cannot be empty\n", 400, pError);
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
			return errorStatus("CL should only include DIGIT\n", 400, pError);
		std::stringstream ss(it->second);
		ss >> _bodyLength; // client_max_body_size check
	}

	it = t_result.header.find("transfer-encoding");
	if (it != t_result.header.end())
	{
		if (_bodyLength != -1)
			return errorStatus("CL and TE are both exist\n", 400, pError);
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
 */
void	Request::parseBody()
{
	/* TE -> transfer-encoding: chunked
	 * CL -> content-length
	 * only TE -> _bodyLength = -1 and _chunked = true -> _bodyLength != -1 and _chunked = true
	 * both No -> _bodyLength = -1 and _chunked = false
	 * only CL -> _bodyLength >= 0 and _chunked = false
	 */
	if (_chunked)
		return parseChunked();
	if (_bodyLength == -1)	// message without content
	{
		t_result.pStatus = pComplete;
		return ;
	}
	t_result.body += _orig;
	if (t_result.body.size() >= _bodyLength)
	{
		t_result.pStatus = pComplete;
		if (t_result.body.size() != _bodyLength)
			t_result.body.erase(_bodyLength); // should be checked
	}
}

/**
 * 	@brief	parsing chunked content
 * 	@param	starting position of the content message
 */
void 	Request::parseChunked()
{
	if (_bodyLength == -1)
		getChunkSize();
	if (t_result.pStatus != pComplete && t_result.pStatus != pError)
		getChunkMessage();
}

/**
 *	@brief	Get chunk message size
 */
void 	Request::getChunkSize()
{
	size_t	pos;

	pos = _orig.find(CRLF);
	if (pos == std::string::npos) // chunked size max also needed
	{
//		if (SOMETHING)
//			return errorStatus("Chunk message first CRLF error", 400, pError);
		return ;
	}

	std::stringstream ss(_orig.substr(0, pos));
	////////////////// hex format should be checked
	ss >> std::hex >> _bodyLength;
	if (_bodyLength == 0) // receiving is done
	{
		if (pos != 1)
			return errorStatus("Chunk message 0 error\n", 400, pError);
		t_result.pStatus = pComplete;
		return ;
	}
	if (_bodyLength < 0)
		return errorStatus("Chunk message size cannot be lower than 0\n", 400, pError);
	_orig.erase(0, pos + 2);
}

/**
 *	@brief	Get chunk message body
 */
void	Request::getChunkMessage()
{
	size_t	pos;

	pos = _orig.find(CRLF);
	if (pos == std::string::npos)
	{
//		if (SOMETHING)
//			return errorStatus("Chunk message second CRLF error", 400, pError);
		return ;
	}

	if (pos != _bodyLength)
		return errorStatus("Chunk message length doesn't match\n", 400, pError);
	t_result.body += _orig.substr(0, pos);
	_orig.erase(0, pos + 2);
	_bodyLength = -1;
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
	std::cout << "[Result Info]" <<
				"\nMethod:" << t_result.method <<
				"\nVersion:" << t_result.version <<
				"\nStatus:" << t_result.status <<
				"\npStatus:" << t_result.pStatus <<
				"\nClose:" << t_result.close <<
				"\nBody:" << t_result.body <<
				"\nPath:" << t_result.path <<
				"\nQuery:" << t_result.query <<
				"\nHost:" << t_result.host
				;
	std::cout << " \n[Header Info]\n";
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

Request::Request(): _bodyLength(0), _chunked(false) {
	t_result.pStatus = pRequest;
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
	t_result.pStatus = orig.t_result.pStatus;
	t_result = orig.t_result;
	return (*this);
}