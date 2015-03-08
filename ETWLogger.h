#ifndef ETWLOGGER_H
#define ETWLOGGER_H
#include "AbstractLogger.h"
#include <string>
#include <windows.h>
#include <evntprov.h>

class ETWLogger final: public AbstractLogger
{
public:
	ETWLogger();
	virtual void log(const std::string& msg) override;
	virtual void err(const std::string& msg) override;
	virtual ~ETWLogger() override;
private:
	REGHANDLE m_registration_handle;
};

#endif
