#include "configParser/Config.hpp"

#include <iostream>
#include <stdexcept>
#include <ctime>
#include <string>
#include <exception>

Config	g_conf("configFiles/default.conf");

int main(int argc, char** argv) {
	if (argc == 2) {
		std::string path = argv[1];
		if ((path.compare(path.find('.'), 5, ".conf")) != 0) {
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
    return 0;
}
