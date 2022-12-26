#include "Request.hpp"

static int	checkHeader(std::string &mHeader);

int	parseHeader(Request &request, size_t &prev)
{
	std::string	mOrig;
	std::string	mHeader;
	size_t		next;

	mOrig = request.getOrig();
	if (mOrig[prev + 2] == SP)
		return errorStatus("# Header Line Error <WhiteSpace>\n", 400);
	next = mOrig.find("\r\n\r\n", prev + 2);
	if (next == std::string::npos)
		return errorStatus("# Header Line Error <Npos>\n", 400);

	mHeader = mOrig.substr(prev + 2, next - prev);
	if (checkHeader(mHeader) < 0)
		return errorStatus("# Header Line Error <Colon or \\r\\n>\n", 400);
	request.setHead(mHeader);
	prev = next + 4;
	return (0);
}

static int	checkHeader(std::string &mHeader)
{
	size_t	pos_nl_prev;
	size_t	pos_nl_next;
	size_t	pos_colon;
	std::string fieldName;
	std::string fieldValue;

	pos_nl_prev = -1;
	while (1) {
		pos_colon = mHeader.find(':', pos_nl_prev + 1);
		if (pos_colon == std::string::npos) {
			if (mHeader[pos_nl_prev + 1] != '\0')
				return (-1);
			break ;
		}
		if (mHeader.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1).find(SP) != std::string::npos)
			return (-1); // SP found between the header field_name and colon
		pos_nl_next = mHeader.find('\n', pos_colon + 1);
		if (pos_nl_next == std::string::npos || mHeader[pos_nl_next - 1] != '\r')
			return (-1);
		fieldName = mHeader.substr(pos_nl_prev + 1, pos_colon - pos_nl_prev - 1);
		fieldValue = mHeader.substr(pos_colon + 1, pos_nl_next - pos_colon - 2); // whitespace ?
//		header[fieldName] = fieldValue;
		pos_nl_prev = pos_nl_next;
	}
	return (0);
}
