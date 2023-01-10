#include "cgi.hpp"

bool isCgi(std::string &target)
{
	size_t pos;

	pos = target.rfind(".php");
	if (pos != std::string::npos)
		return true;
	pos = target.rfind(".py");
	if (pos != std::string::npos)
		return true;
	return false;
}

/*
 * query에서 ?가 포함인가?
 * path_info에는 .py같은 CGI 확장자로 끝나는 정보를 담아야한다?
 *
 * to_string은 c+11부터이다
 *
 */

void setEnvp(std::string &envp, const Request &request)
{
	char	pathbuf[PROC_PIDPATHINFO_MAXSIZE];
	char	*dname;

	if (proc_pidpath(getpid(), pathbuf, sizeof(pathbuf)) < 0)
		; // err
	else
	{
		dname = dirname(pathbuf);
		if (!dname)
			; // err
	}

	envp[0] += ; // AUTH_TYPE
	envp[1] += request.t_result.body.size() > 0 ? std::to_string(request.t_result.body.size()) : ""; // CONTENT_LENGTH
	if (request.t_result.header.count("content-type") > 0) // CONTENT_TYPE
		envp[2] += request.t_result.header["content-type"];
	else
		envp[2] += "null";
	// GATEWAY_INTERFACE
	if (request.t_result.path.size() > 0)
	{
		envp[4] += request.t_result.path; // PATH_INFO
		envp[5] += dname + ; // PATH_TRANSLATED = DOCUMENT_ROOT + PATH_INFO
		envp[6] += request.t_result.query; // QUERY_STRING
	}
	envp[7] += std::to_string((request.t_result.method == 0) ? "GET" : ((request.t_result.method == 1) ? "POST" : "DELETE"));
	envp[8] += ; // SCRIPT_NAME
	envp[9] += ; // SERVER_NAME
	envp[10] += ; // SERVER_PORT
	// SERVER_PROTOCOL
	envp[12] += "42Webserver/1.0"; // SERVER_SOFTWARE
}

int main(int argc, char *argv[], const Request &request)
{
	pid_t	pid;
	int 	fd_result;
	int 	fds[2];

	///////////////
	std::string envp[13] = {
			"AUTH_TYPE=",
			"CONTENT_LENGTH=",
			"CONTENT_TYPE=",
			"GATEWAY_INTERFACE=CGI/1.1",
			"PATH_INFO=",
			"PATH_TRANSLATED=",
			"QUERY_STRING=",
			"REQUEST_METHOD=",
			"SCRIPT_NAME=",
			"SERVER_NAME=", // from config
			"SERVER_PORT=", // from config
			"SERVER_PROTOCOL=HTTP/1.1",
			"SERVER_SOFTWARE=",
	};
	///////////////

	setEnvp(envp, request);

	fd_result = open("../parseRequest/cgi_result_", O_WRONLY | O_CREAT | O_TRUNC);

	if ((pid = fork()) < 0)
		exit(1); // Error

	if ((pipe(fds)) < 0)
		exit(1);

	if (pid == 0) // child
	{
		dup2(fd_result, 1);

		if (execve("../parseRequest/form-get.php", argv, const_cast<char**>(envp)) < 0)
		{
			std::cout << "Exec Failed\n";
			exit(1);
		}
	}
	else // parent
	{
		;
	}
}