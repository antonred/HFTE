
#ifndef HFTE_LOGGING_H_
#define HFTE_LOGGING_H_

/*
 * Logging through spdlog https://github.com/gabime/spdlog
 *
 * Output to console & files
 *
 */

//#define NDEBUG
#include <assert.h>

//! spdlog tweaks described in <spdlog/tweakme.h>
#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON
#define SPDLOG_NO_NAME
#define SPDLOG_NO_REGISTRY_MUTEX
#define SPDLOG_NO_ATOMIC_LEVELS

#include <spdlog/spdlog.h>

namespace HFTE
{
	//! spdlog endpoints for logging
	extern "C" std::shared_ptr<spdlog::logger> console;
	extern "C" std::shared_ptr<spdlog::logger> logfile;

	//! compile time macros for debug output to console
	#define LOGT(fmt, args...) SPDLOG_TRACE(HFTE::console, fmt, ##args)
	#define LOGD(fmt, args...) SPDLOG_DEBUG(HFTE::console, fmt, ##args)

	/*!
	 * @param[in] "/home/hft/logs/" or nullptr
	 * @param[in] force flushing buffer to disk
	 */
	void StartLog(const char *log_files_path = nullptr, bool force_flush = true);
}

#endif /* HFTE_LOGGING_H_ */
