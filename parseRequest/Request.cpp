#include "Request.hpp"

Request::Request() {}

Request::~Request() {}

Request::Request(const Request &orig) {}

Request& Request::operator=(const Request &orig) {
	return (*this);
}

std::string Request::getOrig() { return (_orig); }
std::string Request::getHead() { return (_head); }
std::string Request::getBody() { return (_body); }
std::string	Request::getTarget() { return (_target); }
std::string	Request::getVersion() { return (_version); }
int 		Request::getMethod() { return (_method); }
int 		Request::getStatus() { return (_status); }

void 		Request::setOrig(std::string &orig) {
	_orig = orig;
}

void		Request::setHead(std::string &head) {
	_head = head;
}

void		Request::setBody(std::string &body) {
	_body = body;
}

void 		Request::setTarget(const std::string &target) {
	_target = target;
}

void		Request::setVersion(const std::string &version) {
	_version = version;
}

void 		Request::setMethod(int method) {
	_method = method;
}

void 		Request::updateStatus(int status) {
	_status = status;
}