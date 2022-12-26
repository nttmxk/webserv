#include "Request.hpp"

int	printError(std::string &message)
{
	std::cout << message;
	return (-1);
}

void	Request::errorStatus(std::string message, int status, int pStatus)
{
	updateStatus(status, pStatus);
	std::cout << message;
}

void	printRequest(Request &request)
{
	std::cout <<
			  "[Request Line]" <<
			  "\nMethod:" << request.getMethod() <<
			  "\nTarget:" << request.getTarget() <<
			  "\nVersion:" << request.getVersion() <<
			  "\n[Header Info]\n" << request.getHead() <<
			  "\n[Body Info]\n" << request.getBody() <<
			  std::endl;
}