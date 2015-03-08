#include "FileLogger.h"

FileLogger::FileLogger(const std::string& filename) : of(filename)
{
}

void FileLogger::log(const std::string& msg) {
	std::lock_guard<std::mutex> lock{ mutex };
	of << "[Log]: " << msg << '\n';
}

void FileLogger::err(const std::string& msg) {
	std::lock_guard<std::mutex> lock{ mutex };
	// flush stdout immediately with std::endl, 
	// because when error message is sent
	// program termination is possible soon.
	of << "[Error]: " << msg << std::endl;
}
