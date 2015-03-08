#ifndef ABSTRACTLOGGER_H
#define ABSTRACTLOGGER_H
#include <string>

class AbstractLogger
{
public:
	virtual ~AbstractLogger() = default;
	virtual void log(const std::string& msg) = 0;
	virtual void err(const std::string& msg) = 0;
};

#endif
