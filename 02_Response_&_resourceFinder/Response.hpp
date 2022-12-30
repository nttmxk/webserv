/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Define.hpp"
#include "InfoServer.hpp"


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


/*

- HTTP/1.1 200 OK
- connection : keep-alive / close
- Date : Sat,  2022 14:28:02 GMT
- Server : webserv
- content-length : 424242
- Content-type : text/html / etc..
- CR/LF
- <html> </html>

*/

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

class Response
{
	private:
		HttpResInfo &_info;

	public:
		Response();
		~Response();
		void responseToClient(int clientSocket, InfoServer &serverInfo);
		std::string makeResponseMsg();

		HttpResInfo getResponseInfo();
};




#endif
