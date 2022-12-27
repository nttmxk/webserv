#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <algorithm>
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
		pRequest = 0,
		pHeader,
		pBody,
		pComplete,
		pError
	};
	enum {
		GET = 0,
		POST,
		DELETE
	};
	std::map<std::string, std::string>	header; // what if there's more than one fieldValue ?
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

	void		parseMessage();
	void		printRequest();

	Request();
	~Request();
	Request(const Request &orig);
	Request& operator=(const Request &orig);

private:
	std::string	_orig;
	std::string	_head; // no need?
	std::string	_body;
	std::string	_target;
	std::string	_version;
	int			_status;
	int 		_pStatus;
	int			_method; // GET POST DELETE
	void		parseStartLine(size_t &pos);
	void		checkVersion();
	void		parseControl(std::string &mControl, std::string method);
	void		parseHeader(size_t &prev);
	void		checkHeader();
	void		parseBody(size_t &prev);
	void 		updateStatus(int status, int pStatus);
	void		errorStatus(std::string message, int status, int pStatus);
};

#endif
