#ifndef FILELOGGER_H
#define FILELOGGER_H
#include "AbstractLogger.h"
#include <string>
#include <fstream>
#include <mutex>

class FileLogger final : public AbstractLogger
{
public:
	FileLogger(const std::string& filename);
	virtual void log(const std::string& msg) override;
	virtual void err(const std::string& msg) override;
private:
	std::ofstream of;
	std::mutex mutex;
};

#endif
