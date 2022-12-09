#include "Request.hpp"

static int	checkHeader(std::string &mHeader);

int	parseHeader(Request &request, size_t &prev)
{
	std::string	mOrig;
	std::string	mHeader;
	size_t		next;

	mOrig = request.getOrig();
	if (mOrig[prev + 2] == ' ')
		return printError("# Header Line Error <WhiteSpace>\n");
	next = mOrig.find("\r\n\r\n", prev + 2);
	if (next == std::string::npos)
		return printError("# Header Line Error <Npos>\n");

	mHeader = mOrig.substr(prev + 2, next - prev);
	if (checkHeader(mHeader) < 0)
		return printError("# Header Line Error <Colon or \\r\\n>\n");
	request.setHead(mHeader);
	prev = next + 4;
	return (0);
}

static int	checkHeader(std::string &mHeader)
{
	size_t	pos_nl;
	size_t	pos_colon;

	pos_nl = -1;
	while (1){
		pos_colon = mHeader.find(':', pos_nl + 1);
		if (pos_colon == std::string::npos){
			if (mHeader[pos_nl + 1] != '\0')
				return (-1);
			break ;
		}
		if (mHeader.substr(pos_nl + 1, pos_colon - pos_nl - 1).find(' ') != std::string::npos)
			return (-1); // SP found between the header field_name and colon
		pos_nl = mHeader.find('\n', pos_colon + 1);
		if (pos_nl == std::string::npos || mHeader[pos_nl - 1] != '\r')
			return (-1);
	}
	return (0);
}
