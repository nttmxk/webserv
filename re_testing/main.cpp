/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#include "HttpServer.hpp"
#include "Config.hpp"
#include "Define.hpp"

int main()
{
	Config config;

	ServerConf conf1("127.0.0.1", 8080, "server_1");
	config.serverConfs.push_back(conf1);
	ServerConf conf2("127.0.0.1", 8081, "server_2");
	config.serverConfs.push_back(conf2);
	ServerConf conf3("127.0.0.1", 8082, "server_2");
	config.serverConfs.push_back(conf3);
	ServerConf conf4("127.0.0.1", 8083, "server_3");
	config.serverConfs.push_back(conf4);

	HttpServer webServer(config);

	try {
		webServer.openServer();
	}
	catch (std::exception& e) {
		return (FAIL);
	}

	webServer.runServer(); //runServer() includes Connection class methods

	return (0);
}
