#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>

/** Request class
 * author: jinoh
 * date: 2022.12.09
 */
class Request {
public:
	std::string	getOrig();
	std::string	getHead();
	std::string	getBody();
	std::string	getControl();
	std::string	getTarget();
	std::string	getVersion();
	int 		getMethod();
	void 		setOrig(std::string &orig);
	void		setHead(std::string &head);
	void 		setBody(std::string &body);
	void		setControl(std::string &control);
	void 		setTarget(const std::string &target);
	void		setVersion(const std::string &version);
	void 		setMethod(int method);

	Request();
	~Request();
	Request(const Request &orig);
	Request& operator=(const Request &orig);

private:
	std::string	_orig;
	std::string	_control; // no need?
	std::string	_head;
	std::string	_body;
	std::string	_target;
	std::string	_version; // upper class might have it
	int			_method; // GET POST DELETE

};

/*
 * parse_utils.cpp
 */
int		printError(std::string message);
void	printRequest(Request &request);

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
