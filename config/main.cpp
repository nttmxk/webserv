#include "Config.hpp"

#include <iostream>
#include <stdexcept>
#include <ctime>

Config	g_conf("default.conf");

int main(int argc, char** argv) {
	if (argc == 2) {
		try {
			g_conf = Config(argv[1]);
		}
		catch (std::exception& e) {
			std::cerr << "Error: Config" << std::endl;
			return (1);
		}
	}
	try {
		
		//루프
	}
	catch (std::exception& e) {
		std::cerr << "Error: Event" << std::endl;
		return (1);
	}
	
    return 0;
}