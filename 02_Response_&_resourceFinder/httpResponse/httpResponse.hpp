/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <iostream>
#include <string>
#include <map>
#include <sstream>

class TimeStamp;
class StatusLine;
class Request; // defined in request class

// [!] HttpResInfo 구조체를 밖에서 인자로 넣어주고 리턴해주려면 전체 헤더에서 구조체 타입을 선언 해줘야 함. 회의 필요.
typedef  struct HttpResInfo
{
	StatusLine statusLine;
	TimeStamp date;
	std::string server;
	TimeStamp lastModified;
	std::string contentLocation; // 콘텐츠 협상 후 실제 로케이션... /user.html or /user.xml
	// std::string location; // 300번대 사용하는 리다이렉션 용 URL 콘텐츠 협상 전... /user
	// std::string eTag; //for cache
	std::string transferEncoding; //chunked etc
	unsigned int contentLength;
	std::string contentType; //text/html etc
	std::string contentEncoding; //gzip etc
	std::string connection; //keep-alive etc
	std::string body; // HTML Body :  <body> ... </body>

} HttpResInfo;

class HttpResponse
{
	private:
		HttpResInfo &_info;
		std::map<std::string, std::string> _header;

	public:
		/* cannonical form */
		HttpResponse(Request &reqInfo); //take reference
		~HttpResponse();
		HttpResponse(HttpResponse const &obj);
		HttpResponse &operator=(HttpResponse const &obj);

		/* response header */
		std::map<std::string, std::string> getHeader();

		/* response to be transmitted to the client */
		HttpResInfo getResponseInfo();
	public:
		/* verify resource location accessible */
		bool accessResource(std::string url);
		/* send response */
		void sendResToClient(); // [!] client에 response (인코딩된 파일 및 헤더, html)를 보내는(write()/send()) 역할은 Connection 객체가 하는게 좋을지 회의 필요
};
//-------------------------------------------------------------------------------


/************************************************************/
/*** Class DataType define **********************************/
/************************************************************/

class StatusLine
{
	private:
		std::string _httpVersion; // HTTP/1.1
		int _statusCode; // 200, 300, 404 etc..
		std::string _reasonPhrase; // Not Found, Bad Request ...

	public:
		/* cannonical */
		StatusLine(std::string httpVersion, int statusCode, std::string reasonPhrase);
		StatusLine(StatusLine const &obj);
		~StatusLine();
		StatusLine &operator=(StatusLine const &obj);
		/* setter, getter */
		void setHttpVersion(std::string httpVersion);
		void setStatusCode(int statusCode);
		void setReasonPhrase(std::string reasonPhrase);
		std::string getHttpVersion() const;
		int getStatusCode() const;
		std::string getReasonPhrase() const;
};

class TimeStamp
{
	private:
		std::string _weekDay;
		int _date;
		std::string _month;
		int _year;
		int _hour;
		int _min;
		int _sec;
		std::string _timeZone;

	public:
		/* cannonicatl */
		TimeStamp(std::string weekDay, int date, std::string month, int year, int hour, int min, int sec, std::string timeZone);
		~TimeStamp();
		TimeStamp(TimeStamp const &obj);
		TimeStamp &operator=(TimeStamp const &obj);
		/* setter, getter */
		void setWeekDay(std::string weekDay);
		void setDate(int date);
		void setMonth(std::string month);
		void setYear(int year);
		void setHour(int hour);
		void setMin(int min);
		void setSec(int sec);
		void setTimeZone(std::string timeZone);
		std::string const makeTimeStampString() const;
};

#endif
//-------------------------------------------------------------------------------



/************************************************************/
/*** Additional Info  ***************************************/
/************************************************************/


/*
string		version				Request 객체를 참고해도 되지 않을까 하는 생각이 듦
	int		status_code			상태코드(200, 404, etc…)
	map<str, str>	header				헤더정보를 fieldKey, fieldValue 로 들고있는게 좋을거같음
	string		body				response body

	void		updateStatus(int status)	상태코드를 업데이트
	string		statusText(int status)		returns status text (OK, Bad request, etc…)
	void		sendResponse			응답 메세지를 송신
*/



/*
TYPICAL TYPE OF HTTP RESPONSE :

HTTP/1.1 200 OK
Date: Sat, 09 Oct 2010 14:28:02 GMT
Server: Apache
Last-Modified: Tue, 01 Dec 2009 20:18:22 GMT
ETag: "51142bc1-7449-479b075b2891b"
Accept-Ranges: bytes
Content-Length: 29769
Content-Type: text/html

<!DOCTYPE html>… (here come the 29769 bytes of the requested web page)
*/

// ref : https://developer.mozilla.org/en-US/docs/Web/HTTP/Overview
// ref : https://www.tutorialspoint.com/http/http_responses.htm



/* another example :
200 OK
Access-Control-Allow-Origin: *
Connection: Keep-Alive
Content-Encoding: gzip
Content-Type: text/html; charset=utf-8
Date: Mon, 18 Jul 2016 16:06:00 GMT
Etag: "c561c68d0ba92bbeb8b0f612a9199f722e3a621a"
Keep-Alive: timeout=5, max=997
Last-Modified: Mon, 18 Jul 2016 02:36:04 GMT
Server: Apache
Set-Cookie: mykey=myvalue; expires=Mon, 17-Jul-2017 16:06:00 GMT; Max-Age=31449600; Path=/; secure
Transfer-Encoding: chunked
Vary: Cookie, Accept-Encoding
X-Backend-Server: developer2.webapp.scl3.mozilla.com
X-Cache-Info: not cacheable; meta data too large
X-kuma-revision: 1085259
x-frame-options: DENY
https://gmlwjd9405.github.io/2019/01/28/http-header-types.html
*/


/* Response status codde
https://developer.mozilla.org/ko/docs/Web/HTTP/Status
*/



//defined in request class .. to be replaced by Request.hpp
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
