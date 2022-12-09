#include "Request.hpp"

int	printError(std::string message)
{
	std::cout << message;
	return (-1);
}

void	printRequest(Request &request)
{
	std::cout <<
			  "[Request Line]" <<
			  "\n" << request.getControl() <<
			  "\nMethod:" << request.getMethod() <<
			  "\nTarget:" << request.getTarget() <<
			  "\nVersion:" << request.getVersion() <<
			  "\n[Header Info]\n" << request.getHead() <<
			  "\n[Body Info]\n" << request.getBody() <<
			  std::endl;
}