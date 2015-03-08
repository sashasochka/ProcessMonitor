#include "processmonitor.h"
#include "filelogger.h"
#include "etwlogger.h"
#include <iostream>
#include <cstdlib>
#include <memory>

int main() {
	// ProcessMonitor pm(6084);
	//ProcessMonitor pm(L"C:\\Program Files\\Sublime Text 3\\sublime_text a.txt");
	
	ProcessMonitor pm(L"notepad");
	pm.set_logger(std::make_shared<FileLogger>("log.txt"));
	// pm.set_logger(std::make_shared<ETWLogger>());
	std::cout << pm.get_pid() << std::endl;

	// Callbacks for demo purposes
	// Race conditions are possible here, normally use mutexes with std::cout.
	pm.on_proc_start([]{ std::cout << "Proc started" << std::endl;  });
	pm.on_proc_crash([]{ std::cout << "Proc crashed" << std::endl;  });
	pm.on_proc_crash([]{ std::cout << "Proc crashed callback #2" << std::endl;  });
	pm.on_proc_normal_exit([]{ std::cout << "Proc exited normally" << std::endl;  });
	pm.on_proc_manually_stopped([]{ std::cout << "Proc manually stopped" << std::endl;  });
//	pm.stop_process();
	Sleep(2000000);
	std::cout << "Finished successfully" << std::endl;
	return EXIT_SUCCESS;
}
