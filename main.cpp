/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#include "webserv2/HttpServer.hpp"
#include "configParser/Config.hpp"
#include "webserv2/includes/Define.hpp"

int main(int argc, char** argv)
{

	Config config("configFiles/default.conf");

	if (argc == 2) {
		std::string path = argv[1];
		if ((path.compare(path.find('.'), 5, ".conf")) != 0) {
			std::cerr << "Error: Config" << std::endl;
			return (1);
		}
		try {
			config = Config(path);
		}
		catch (std::exception& e) {
			std::cerr << "Error: Config" << std::endl;
			return (1);
		}
	}

	HttpServer webServer(config);

	try {
		webServer.openServer();
		webServer.runServer(); //runServer() includes Connection class methods
	}
	catch (std::exception& e) {
		//Do we need to reponse 5XX err?
		return (FAIL);
	}


	return (0);
}
