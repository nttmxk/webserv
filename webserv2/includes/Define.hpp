/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef DEFINE_HPP
#define DEFINE_HPP

#define SUCCESS 0
#define FAIL -1
#define BUFFER_SIZE 1024

#define IP_ADDRESS "127.0.0.1"
#define PORT 8080

#define MAX_NUM_EVENTS 1024

#define NONE -1
/*
response class
*/
#define PREPARING 0
#define DONE 1
#define CRLF "\r\n"

enum {
	GET,
	POST,
	DELETE
};

#endif
