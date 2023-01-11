/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef HTTPRESINFO_HPP
#define HTTPRESINFO_HPP

#include "../includes/Define.hpp"
#include "../includes/cppLibrary.hpp"

/*=====================================================
- HTTP/1.1 200 OK
- Connection : keep-alive / close
- Date : Sat, 09 Oct 2010 14:28:02 GMT
- Server : webserv
- Content-type : text/html / etc..
- Transfer-Encoding : chunked
- Content-Length : 424242  // if Transfer-Encoding exists, Content-Lengthi will be disregarded

- <html> </html>
=======================================================*/

class HttpResInfo
{
	protected:
		std::string _httpVersion;
		int _statusCode;
		std::string _statusMsg;
		std::string _connection; //keep-alive etc
		std::string _date;
		std::string _server;
		std::string _contentType; //text/html etc
		std::string _transferEncoding; //chunked etc
		unsigned int _contentLength;
		std::string _contentEncoding; //gzip etc
		std::string _resBody; // HTML Body :  <body> ... </body>
		std::map<int, std::string> _statusMap;

	public:
		HttpResInfo()
		: _httpVersion("HTTP/1.1"), _statusCode(0), _statusMsg(""), _connection(""), _date(""), \
		_server("webserv /1.0"), _contentType(""), _transferEncoding(""), _contentLength(0), _contentEncoding(""), _resBody("") { setStatusMsgMap();}
		void setStatusCode(int code) { _statusCode = code; }
		void setStatusMsg(std::string msg) { _statusMsg = msg; }
		void setConnection(std::string connection) { _connection = connection; }
		void setContentType(std::string path_type ) { 
			if (path_type == "html")
				_contentType = "text/html";
			else if (path_type == "txt")
				_contentType = "text/html";
			else
				_contentType = "text/plain";
			// else if (path_type == "html")
			// 	_contentType = "text/html";
			// else if (path_type == "html")
			// 	_contentType = "text/html";
			// else if (path_type == "html")
			// 	_contentType = "text/html";
		}
		void setTransferEncoding(std::string encod ) { _transferEncoding = encod; }
		void setContentLength(unsigned int len) { _contentLength = len; }
		void setContentEncoding(std::string encod ) { _contentEncoding = encod; }
		void setBody(std::string resBody) { _resBody = resBody; }
		void setDate( ) { _date = timeStamp(); }
		std::string getHttpVersion() {return (_httpVersion);}
		int getStatusCode() { return (_statusCode); }
		std::string getStatusMsg(int code) {
			if (_statusMap.find(code) != _statusMap.end())
				return _statusMap[code];
			 return ("nothinf"); 
		}
		std::string getConnection() { return (_connection);}
		std::string getDate() { return (_date); }
		std::string getServer() { return (_server); }
		std::string getContentType() { return (_contentType);}
		std::string getTransferEncoding() { return (_transferEncoding); }
		unsigned int getContentLength() { return (_contentLength); }
		std::string getContentEncoding() { return (_contentEncoding); }
		std::string getResponseBody() {return (_resBody); }


		
	public:
		std::string timeStamp() {
			std::string days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sum"};
			std::string months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
			std::time_t timeBuff = std::time(NULL);
			struct tm *time = std::localtime(&timeBuff);
			std::stringstream ss;
			ss << days[time->tm_wday] << ", " << time->tm_mday << " " << months[time->tm_mon] << " " << 1900 + time->tm_year
				<< " ";
			if (time->tm_hour < 10)
				ss << "0";
			ss << time->tm_hour << ":";
			if (time->tm_min < 10)
				ss << "0";
			ss << time->tm_min << ":";
			if (time->tm_sec < 10)
				ss << "0";
			ss << time->tm_sec << " GMT";
			return (ss.str());
		}
		void setStatusMsgMap()
		{
			_statusMap[100] = "Continue";
			_statusMap[101] = "Switching Protocols";
			_statusMap[200] = "OK";
			_statusMap[201] = "Created";
			_statusMap[202] = "Accepted";
			_statusMap[203] = "Non-Authoritative Information";
			_statusMap[204] = "No Content";
			_statusMap[205] = "Reset Content";
			_statusMap[206] = "Partial Content";
			_statusMap[300] = "Multiple Choices";
			_statusMap[301] = "Moved Permanently";
			_statusMap[302] = "Found";
			_statusMap[303] = "See Other";
			_statusMap[304] = "Not Modified";
			_statusMap[305] = "Use Proxy";
			_statusMap[307] = "Temporary Redirect";
			_statusMap[400] = "Bad Request";
			_statusMap[401] = "Unauthorized";
			_statusMap[402] = "Payment Required";
			_statusMap[403] = "Forbidden";
			_statusMap[404] = "Not Found";
			_statusMap[405] = "Method Not Allowed";
			_statusMap[406] = "Not Acceptable";
			_statusMap[407] = "Proxy Authentication Required";
			_statusMap[408] = "Request Time-out";
			_statusMap[409] = " Conflict";
			_statusMap[410] = " Gone";
			_statusMap[411] = " Length Required";
			_statusMap[412] = " Precondition Failed";
			_statusMap[413] = " Request Entity Too Large";
			_statusMap[414] = " Request-URI Too Large";
			_statusMap[415] = " Unsupported Media Type";
			_statusMap[416] = " Requested range not satisfiable";
			_statusMap[417] = " Expectation Failed";
			_statusMap[500] = "Internal Server Error";
			_statusMap[501] = "Not Implemented";
			_statusMap[502] = "Bad Gateway";
			_statusMap[503] = "Service Unavailable";
			_statusMap[504] = "Gateway Time-out";
			_statusMap[505] = "HTTP Version not supported";
		}

};

#endif
