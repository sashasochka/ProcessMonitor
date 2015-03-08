#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <windows.h>
#include <string>
#include <list>
#include <functional>
#include <memory>
#include <wrl/wrappers/corewrappers.h>

#include "abstractlogger.h"

class ProcessMonitor final
{
public:
	using Callback = std::function<void()>;

	// With std::list (instead of std::vector)
	// it's potentially easier to implement 
	// event unsubscription because of O(1) removal time. 
	// In this case std::vector would work as well
	using Callbacks = std::list<Callback>;

	enum class ProcessState {
		Running,
		Stopped,
		Restarting
	};

	// Custom exception class, equivalent of std::runtime_error
	// Don't use aliasing - prefer separate types for 
	// type-based error handling in try/catch
	// Use c++11 constructor inheritance
	struct Error : std::runtime_error  {
		using runtime_error::runtime_error;
	};

	// PID of fake/virtual idle process
	static const uint32_t invalid_pid{ 0 };

	// attach to existing process
	ProcessMonitor(uint32_t pid);

	// create new process and attach to it
	ProcessMonitor(const std::wstring& path, const std::wstring& args = L"");

	// get process id or `invalid_pid` if it's not in running state
	uint32_t get_pid();

	// get process handle or INVALID_HANDLE_VALUE if it's not in running state
	HANDLE get_handle();

	// get state of the process (one of the ProcessState values)
	ProcessState get_state();

	// Stop process manually, true if successfull
	bool stop_process(uint32_t error_code = 0);

	// Start process manually, true if successfull
	bool start_process();

	void set_logger(std::shared_ptr<AbstractLogger> logger);


	// Add calbacks
	void on_proc_start(const Callback& cb);
	void on_proc_crash(const Callback& cb);
	void on_proc_normal_exit(const Callback& cb);
	void on_proc_manually_stopped(const Callback& cb);

	// Delete default constructor implementations so there
	// are no accidental resource leaks / double freeing.
	// Ideally should be implemented, but not stated in task description
	ProcessMonitor(const ProcessMonitor&) = delete;
	ProcessMonitor(ProcessMonitor&&) = delete;
	ProcessMonitor& operator=(const ProcessMonitor&)& = delete;
	ProcessMonitor& operator=(ProcessMonitor&&)& = delete;

	~ProcessMonitor();

private:
	// works with 32/64/WOW64 processes
	bool get_cmd_line_from_process();
	void process_exited();

	// Send log message to logger
	void log(const std::string& msg);

	// Send error message to logger and throw exception
	void err(const std::string& msg);

	void run_callbacks(const Callbacks& callbacks);
	static void CALLBACK ProcessExitedCallback(void* lpParam, unsigned char);

	// Slim-Read-Write lock (Windows equivalent of std::shared_mutex in C++14)
	Microsoft::WRL::Wrappers::SRWLock m_lock;
	std::shared_ptr<AbstractLogger> m_logger;
	HANDLE m_process_exit_wait_handle{0};
	HANDLE m_handle{0};
	ProcessState m_state;
	// command line used for restarting processes
	std::wstring m_cmd;

	Callbacks m_on_proc_start_callbacks;
	Callbacks m_on_proc_crash_callbacks;
	Callbacks m_on_proc_normal_exit_callbacks;
	Callbacks m_on_proc_manually_stopped_callbacks;
};

#endif PROCESSMONITOR_H
