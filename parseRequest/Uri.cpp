#include "Uri.hpp"

void Uri::parseTarget(std::string &uri) // rfc 3986
{
	size_t pos;

	pos = 0;
	if (isalpha(uri[0]))
	{
		checkScheme(uri, pos);
		checkHier(uri, ++pos);
	}
	else if (uri[0] == '/')
	{
		checkPath(uri, pos);
		if (_valid && uri[pos] == '?')
			checkQuery(uri, pos);
	}
	else
		_valid = false;
}

void Uri::checkScheme(std::string &uri, size_t &pos)
{
	pos = uri.find(':');
	if (pos == std::string::npos)
	{
		_valid = false;
		return;
	}
	for (int i = 0; (unsigned long)i < pos; ++i)
	{
		if (!isCharset(ALPHA DIGIT "+.-", uri[i]))
		{
			_valid = false;
			return ;
		}
	}
	_scheme = uri.substr(0, pos);
}

/**
 *	@brief
 *	@param	Uri, index after ':'
 */
void Uri::checkHier(std::string &uri, size_t &pos)
{
	if (!_valid || uri[pos] != '/')
	{
		_valid = false;
		return ;
	}
	if (uri[pos + 1] == '/')
		checkAuth(uri, pos);
	if (_valid)
		checkPath(uri, pos);
	if (_valid && uri[pos] == '?')
		checkQuery(uri, pos);
}

void Uri::checkAuth(std::string &uri, size_t &pos)
{
	size_t start;
	size_t start_port;

	pos += 2;
	start = pos;
	while (pos < uri.size() && _valid && (uri[pos] != ':' && uri[pos] != '/'))
	{
		if (isCharset(UNRESERVED SUB_DELIMS, uri[pos])) // user : password is deprecated, so don't accept it (chrome 59)
			pos++;
		else if (uri[pos] == '%')
			decodeHex(uri, pos);
		else
			_valid = false;
	}
	_valid = (start != pos);
	if (!_valid)
		return ;
	_host = uri.substr(start, pos - start);
	if (uri[pos] == ':')
	{
		start_port = ++pos;
		while (pos < uri.size() && isdigit(uri[pos]))
			pos++;
		_port = uri.substr(start_port, pos - start_port);
	}
}

void Uri::checkPath(std::string &uri, size_t &pos)
{
	size_t	start;

	start = pos;
	while (pos < uri.size() && _valid && uri[pos] != '?')
	{
		if (isCharset(PCHAR, uri[pos]) || uri[pos] == '/')
			pos++;
		else if (uri[pos] == '%')
			decodeHex(uri, pos);
		else
			_valid = false;
	}
	_path = uri.substr(start, pos - start);
}

void Uri::checkQuery(std::string &uri, size_t pos)
{
	size_t	start;

	start = pos;
	while (pos < uri.size() && _valid)
	{
		if (isCharset(PCHAR, uri[pos]) || uri[pos] == '/' || uri[pos] == '?')
			pos++;
		else if (uri[pos] == '%')  // it is sometimes better for usability to avoid percent-encoding those characters ???
			pos++; // it sounds vague...
		else if (uri[pos] == '#')
			break ;
		else
			_valid = false;
	}
	_query = uri.substr(start, pos - start);
}

void Uri::decodeHex(std::string &uri, size_t pos)
{
	if (pos + 2 >= uri.size() || !isCharset(HEXDIG, uri[pos + 1]) || !isCharset(HEXDIG, uri[pos + 2]))
	{
		_valid = false;
		return ;
	}
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << uri.substr(pos + 1, 2);
	ss >> x;
	uri.replace(pos, 3, 1, x); // %FF -> H
}

void Uri::encodeToHex(std::string &uri)
{
	for (size_t i = 0; i < uri.size(); ++i)
	{
		if (isCharset(RESERVED, uri[i]))
		{
			std::stringstream ss;
			ss << std::hex << (int)uri[i];
			std::string hex = ss.str();
			if (hex.size() == 1)
				hex = '0' + hex;
			uri.replace(i, 1, '%' + hex);
			i += 2;
		}
	}
}

bool Uri::isCharset(std::string charset, char c)
{
	if (charset.find(c) != std::string::npos)
		return true;
	return false;
}

Uri::Uri() :_valid(true) {}

Uri::~Uri() {}

Uri::Uri(const Uri &orig) {
	*this = orig;
}

Uri& Uri::operator=(const Uri &orig) {
	(void)orig;
	return (*this);
}
