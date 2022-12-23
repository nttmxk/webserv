//-----------------//
/* coded by jibang */
//-----------------//

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <iostream>
#include <string>
#include <map>

class TimeStamp;
class StatusLine;

// [!] HttpResInfo 구조체를 밖에서 인자로 넣어주고 리턴해주려면 전체 헤더에서 구조체 타입을 선언 해줘야 함. 회의 필요.
typedef  struct HttpResInfo
{
	StatusLine statusLine;
	std::string serverName;
	TimeStamp lastModified;
	std::string eTag; //for cache
	unsigned int contentLength;
	std::string contentType;
	std::string connection;
	std::string body;

} HttpResInfo;

class HttpResponse
{
	private:
		HttpResInfo &info;
		std::map<std::string, std::string> header;

	public:
		/* cannonical form */
		HttpResponse();
		HttpResponse(HttpResInfo &info); //take reference
		~HttpResponse();
		HttpResponse(HttpResponse const &obj);
		HttpResponse &operator=(HttpResponse const &obj);

		/* response header */
		std::map<std::string, std::string> getHeader();

		/* response to be transmitted to the client */
		HttpResInfo getResponseInfo();

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
		std::string httpVersion; // HTTP/1.1
		int statusCode; // 200, 300, 404 etc..
		std::string reasonPhrase; // Not Found, Bad Request ...

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
		std::string getHttpVersion();
		int getStatusCode() const;
		std::string getReasonPhrase() const;
		std::string getStatusLineString() const;
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
		std::string const getTimeStampString() const;
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

