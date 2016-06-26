
#include "Logger.h"

namespace HFTE
{
	std::shared_ptr<spdlog::logger> console;
	std::shared_ptr<spdlog::logger> logfile;

	void StartLog(const char *log_files_path, bool force_flush)
	{
		spdlog::set_pattern("[%H:%M:%S.%f] [thread %t] %v");

		console = spdlog::stdout_logger_mt("console", false);
		spdlog::set_level(spdlog::level::trace);
		//console->set_level(spdlog::level::debug);
		console->info("Logger info test");
		console->debug("Logger debug test");
		LOGD("Logger MACRO LOGD TEST");
		LOGT("Logger MACRO LOGT TEST");

		if (log_files_path != nullptr)
		{
			std::string file_name = log_files_path;
			//file_name += "log";

			size_t queue_size = 65536;
			spdlog::set_async_mode(queue_size);
			logfile = spdlog::daily_logger_st("async_file_logger", file_name.c_str(), 0, 0, force_flush);
		}
	}
}
