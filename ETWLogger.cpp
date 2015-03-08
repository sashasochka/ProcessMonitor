#include "ETWLogger.h"
#include "etw.h"

ETWLogger::ETWLogger() {
	m_registration_handle = 0;
	auto status = EventRegister(
		&ProviderGuid,      // GUID that identifies the provider
		nullptr,               // Callback not used
		nullptr,               // Context noot used
		&m_registration_handle // Used when calling EventWrite and EventUnregister
		);
}

void ETWLogger::log(const std::string& msg) {
	EVENT_DATA_DESCRIPTOR descr;
	EventDataDescCreate(&descr, msg.c_str(), static_cast<ULONG>(msg.size()));

	auto status = EventWrite(
		m_registration_handle,
		&LogEvent, 
		1,  
		&descr
		);
	if (status != 0) DebugBreak();
}

void ETWLogger::err(const std::string& msg) {
	EVENT_DATA_DESCRIPTOR descr;
	EventDataDescCreate(&descr, msg.c_str(), static_cast<ULONG>(msg.size()));
	auto status = EventWrite(
		m_registration_handle,
		&ErrEvent,
		1,
		&descr
		);
	if (status != 0) DebugBreak();
}

ETWLogger::~ETWLogger() {
	EventUnregister(m_registration_handle);
}
