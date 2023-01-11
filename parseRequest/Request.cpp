#include "Request.hpp"

/**
 *	@brief	Set request class to empty status
 *	@when	After send a response to client
 */
void 		Request::clearRequest()
{
	_buf.clear();
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
	t_result.orig += message;
	_buf += message;
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

	pos = _buf.find(CRLF, 0);
	if (pos == std::string::npos)
	{
		if (_buf.size() > SIZE_MAX_REQ)
			return errorStatus("# Request Line Too long\n", 400, pError);
		return ;
	}

	mControl = _buf.substr(0, pos);
	_buf.erase(0, pos + 2);

	if (std::count(mControl.begin(), mControl.end(),' ') != 2)
		return errorStatus("# Request Line Error <WhiteSpace>\n", 400, pError);
	if (mControl.length() < 14) // The shortest message "GET / HTTP/1.0" is 14 characters
		return errorStatus("# Request Line Too short\n", 400, pError);

	checkMethod(mControl);

	pos = mControl.find(SP);
	_target = mControl.substr(0, pos);
	t_result.target = _target;
	_version = mControl.erase(0, pos + 1);

	checkTarget();
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
	return errorStatus("# Not Implemented\n", 501, pError); // Not Implemented
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
	if (_target.size() > SIZE_MAX_URI)
		return errorStatus("# URI Too long\n", 414, pError);

	Uri uriParser;
	uriParser.parseTarget(_target);
	if (!uriParser._valid)
		return errorStatus("# URI Error in UriParser\n", 400, pError);
	t_result.host = uriParser._host;
	t_result.port = uriParser._port;
	t_result.path = uriParser._path;
	t_result.query = uriParser._query;

}

/**
 * 	@brief	checking http version
 */
void	Request::checkVersion()
{
	if (_version.find(SP) != std::string::npos || _version.size() != 8 || _version.compare(0, 7, "HTTP/1.") != 0)
		return updateStatus(400, pError); // Bad Request
	if (_version.compare("HTTP/1.1") != 0 && _version.compare("HTTP/1.0") != 0)
		return updateStatus(505, pError); // HTTP Version Not Supported
}

/**
 * 	@brief	parsing header
 */
void	Request::parseHeader()
{
	size_t		pos;

	pos = _buf.find(CRLF CRLF);
	if (pos == std::string::npos)
	{
		if (_buf.size() > SIZE_MAX_HEADER)
			return errorStatus("# Header Line Too Long\n", 400, pError);
		return ;
	}
	_head = _buf.substr(0, pos + 2);
	_buf.erase(0, pos + 4);
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
		{
			t_result.close = true;
			return errorStatus("CL should only include DIGIT\n", 400, pError);
		}
		std::stringstream ss(it->second);
		ss >> _bodyLength;
		///////////// client_max_body_size check
		if (it->second.size() > 4 || _bodyLength > SIZE_MAX_BODY)
		{
			t_result.close = true;
			return errorStatus("413 Payload Too Large\n", 413, pError);
		}
	}

	it = t_result.header.find("transfer-encoding");
	if (it != t_result.header.end())
	{
		if (_bodyLength != -1)
		{
			t_result.close = true;
			return errorStatus("CL and TE are both exist\n", 400, pError);
		}
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
	 * both No -> _bodyLength = -1 and _chunked = false -> message without body
	 * only CL -> _bodyLength >= 0 and _chunked = false
	 */
	if (_chunked)
		return parseChunked();
	if (_bodyLength == -1)	// message without content
	{
		t_result.pStatus = pComplete;
		return ;
	}
	t_result.body += _buf;
	_buf.clear();
	if (t_result.body.size() >= SIZE_MAX_BODY)
	{
		t_result.close = true;
		return errorStatus("413 Payload Too Large\n", 413, pError);
	}
	if (t_result.body.size() >= (unsigned long)_bodyLength)
	{
		t_result.pStatus = pComplete;
		if (t_result.body.size() != (unsigned long)_bodyLength)
			t_result.body.erase(_bodyLength); // should be checked
	}
}

/**
 * 	@brief	parsing chunked content
 */
void 	Request::parseChunked()
{
	_chunkReady = true;
	while (_chunkReady && t_result.pStatus == pBody)
	{
		if (_bodyLength == -1)
			getChunkSize();
		if (_bodyLength != -1 && t_result.pStatus == pBody)
			getChunkMessage();
	}
}

/**
 *	@brief	Get chunk message size
 */
void 	Request::getChunkSize()
{
	size_t	pos;

	pos = _buf.find(CRLF);
	if (pos == std::string::npos)
	{
		if (_buf.size() > SIZE_MAX_CHUNK + 1)
			return errorStatus("Chunk message Too Long\n", 400, pError);
		_chunkReady = false;
		return ;
	}

	if (pos > SIZE_MAX_CHUNK)
		return errorStatus("Chunk message Too Long\n", 400, pError);

	std::stringstream ss(_buf.substr(0, pos));
	////////////////// hex format should be checked
	ss >> std::hex >> _bodyLength;
	if (_bodyLength > 4095)
		return errorStatus("Chunk message is bigger than FFF\n", 400, pError);
	if (_bodyLength == 0) // receiving is done
	{
		if (pos != 1)
			return errorStatus("Chunk message 0 error\n", 400, pError);
		t_result.pStatus = pComplete;
		return ;
	}
	if (_bodyLength < 0)
		return errorStatus("Chunk message size cannot be lower than 0\n", 400, pError);
	_buf.erase(0, pos + 2);
}

/**
 *	@brief	Get chunk message body
 */
void	Request::getChunkMessage()
{
	size_t	pos;

	pos = _buf.find(CRLF);
	if (pos == std::string::npos)
	{
		if (_buf.size() > 4095 + 1)
			return errorStatus("Chunk message second CRLF error", 400, pError);
		_chunkReady = false;
		return ;
	}

	if (pos != (unsigned long)_bodyLength)
		return errorStatus("Chunk message length doesn't match\n", 400, pError);
	t_result.body += _buf.substr(0, pos);
	if (t_result.body.size() >= SIZE_MAX_BODY)
	{
		t_result.close = true;
		return errorStatus("413 Payload Too Large\n", 413, pError);
	}
	_buf.erase(0, pos + 2);
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
			  "\nTarget:" << _target << // soon to be deleted
			  "\nVersion:" << _version <<
			  std::endl;
	std::cout << "[Result Info]" <<
				"\nMethod:" << t_result.method <<
				"\nVersion:" << t_result.version <<
				"\nStatus:" << t_result.status <<
				"\npStatus:" << t_result.pStatus <<
				"\nClose:" << t_result.close <<
				"\nBody:" << t_result.body <<
				"\nHost:" << t_result.host <<
				"\nPort:" << t_result.port <<
				"\nPath:" << t_result.path <<
				"\nQuery:" << t_result.query
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

Request& Request::operator=(const Request &orig) { ///////// should be modified
	_buf = orig._buf;
	_head = orig._head;
	_target = orig._target;
	_version = orig._version;
	t_result.pStatus = orig.t_result.pStatus;
	t_result = orig.t_result;
	return (*this);
}
