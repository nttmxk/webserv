#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <algorithm>
# include <map>
# include <sstream>
# define SP ' '
# define HTAB '\t'
# define CRLF "\r\n"
# define DIGIT "0123456789"

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
	struct s_result {
		int method;
		int version; // no need?
		int status;
		int pStatus;
		bool close;
		std::map<std::string, std::string> header;
		std::string body;
		std::string path; // target = (host) + path + query
		std::string query;
		std::string host;
	}	t_result;
	std::string	getHead();
	std::string	getTarget();
	std::string	getVersion();
	void 		clearRequest();

	void		parseMessage(std::string message);
	void		printRequest();

	Request();
	~Request();
	Request(const Request &orig);
	Request& operator=(const Request &orig);

private:
	std::string	_orig;
	std::string	_head;
	std::string	_target;
	std::string	_version;	// no need?
	int 		_bodyLength;
	bool		_chunked;
	void		parseRequestLine();
	void 		checkMethod(std::string &mControl);
	void		validateMethod(std::string &mControl, std::string method);
	void 		checkTarget();
	void		checkVersion();

	void		parseHeader();
	void		tokenizeHeader();
	bool		isOWS(int c);
	void		checkHeader();
	void 		verifyHeader();
	void 		checkHost();
	void 		checkBodyLength();
	void 		checkConnection();

	void		parseBody();
	void		parseChunked();
	void		getChunkSize();
	void 		getChunkMessage();

	void 		updateStatus(int status, int pStatus);
	void		errorStatus(std::string message, int status, int pStatus);
};

#endif
