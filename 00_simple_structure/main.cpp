/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#include "HttpServer.hpp"
#include "Config.hpp"
#include "Define.hpp"

#define IP_ADDRESS "0.0.0.0" //let OS choose default ip address
#define PORT 8080

class Config;

int main()
{
	Config config(IP_ADDRESS, PORT);
	HttpServer webServer(config);

	if (webServer.openServer() == FAIL)
		return (1);

	webServer.runServer(); //runServer() includes Connection class methods

	return (0);
}


