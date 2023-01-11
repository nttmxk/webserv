#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Uri.hpp"
# include <iostream>
# include <algorithm>
# include <map>
# include <sstream>
# define SP ' '
# define HTAB '\t'
# define CRLF "\r\n"
# define DIGIT "0123456789"
# define SIZE_MAX_REQ 6 + SIZE_MAX_URI + 8 // DELETE + URI + VERSION
# define SIZE_MAX_URI 1024
# define SIZE_MAX_HEADER 2048
# define SIZE_MAX_HOST 128 // verifyhost
# define SIZE_MAX_CHUNK 3 // hex FFF = dec 4095
# define SIZE_MAX_BODY 819200000

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
		std::string host;
		std::string port;
		std::string path;
		std::string query;
		std::string target;
		std::string orig;
	}	t_result;

	void 		clearRequest();

	void		parseMessage(std::string message);
	void		printRequest();

	Request();
	~Request();
	Request(const Request &orig);
	Request& operator=(const Request &orig);

private:

	std::string	_buf;
	std::string	_head;
	std::string	_target;
	std::string	_version;	// no need?
	int 		_bodyLength;
	bool		_chunkReady;
	bool		_chunked;
	void		parseRequestLine();
	void 		checkMethod(std::string &mControl);
	void		validateMethod(std::string &mControl, std::string method);
	void 		checkTarget();
	void		checkVersion();

	void		parseHeader();
	void		tokenizeHeader();
	bool		isOWS(int c);
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
