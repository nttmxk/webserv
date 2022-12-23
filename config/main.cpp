#include "Config.hpp"

#include <iostream>
#include <stdexcept>
#include <ctime>
#include <string>
#include <exception>

Config	g_conf("default.conf");

int main(int argc, char** argv) {
	if (argc == 2) {
		std::string path = argv[1];
		if (!(path.compare(path.rfind('.'), 7, ".config"))) {
			std::cerr << "Error: Config" << std::endl;
			return (1);
		}
		try {
			g_conf = Config(path);
		}
		catch (std::exception& e) {
			std::cerr << "Error: Config" << std::endl;
			return (1);
		}
	}
	// try {
		
	// 	//루프
	// }
	// catch (std::exception& e) {
	// 	std::cerr << "Error: Event" << std::endl;
	// 	return (1);
	// }
	
    return 0;
}