#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <map>
# define SP ' '
# define CRLF "\r\n"

/** Request class
 * author: jinoh
 * date: 2022.12.09
 */
class Request {
public:
	enum {
		GET = 0,
		POST,
		DELETE
	};
	map<std::string, std::string>	header; // what if there's more than one fieldValue ?
	std::string	getOrig();
	std::string	getHead();
	std::string	getBody();
	std::string	getTarget();
	std::string	getVersion();
	int 		getMethod();
	int 		getStatus();
	void 		setOrig(std::string &orig);
	void		setHead(std::string &head);
	void 		setBody(std::string &body);
	void 		setTarget(const std::string &target);
	void		setVersion(const std::string &version);
	void 		setMethod(int method);
	void 		updateStatus(int status);

	Request();
	~Request();
	Request(const Request &orig);
	Request& operator=(const Request &orig);

private:
	std::string	_orig;
	std::string	_head; // no need?
	std::string	_body;
	std::string	_target;
	std::string	_version; // upper class might have it
	int			_status;
	int			_method; // GET POST DELETE

};

/*
 * parse_utils.cpp
 */
int		printError(std::string message);
void	printRequest(Request &request);
int		errorStatus(std::string message, int status);

/*
 * parse.cpp
 */
int		parseMessage(Request &request);

/*
 * parse_requestline.cpp
 */
int		parseStartLine(Request &request, size_t &pos);

/*
 * parse_header.cpp
 */
int		parseHeader(Request &request, size_t &prev);

/*
 * parse_body.cpp
 */
int		parseBody(Request &request, size_t &prev);

#endif
