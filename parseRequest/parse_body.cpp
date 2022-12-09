#include "Request.hpp"

int	parseBody(Request &request, size_t &prev)
{
	std::string	mBody;

	mBody = request.getOrig().substr(prev);
	request.setBody(mBody);
	return (0);
}
