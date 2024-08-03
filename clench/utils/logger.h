#ifndef _CLENCH_UTILS_LOGGER_H_
#define _CLENCH_UTILS_LOGGER_H_

#include "basedefs.h"
#include <cstdarg>

namespace clench {
	namespace utils {
		enum class LogLevel {
			Fatal = 0,
			Error,
			Warning,
			Note,
			Info,
			Debug
		};

		typedef void (*Logger)(LogLevel level, const char *message, va_list varArgs);

		CLCUTILS_API void defaultLogger(LogLevel level, const char *message, va_list varArgs);

		extern Logger g_logger;

		CLCUTILS_API void doLogging(LogLevel level, const char *message, ...);
	}
}

#ifdef NDEBUG

	#define CLENCH_LOG(level, message, ...)

#else

	#define CLENCH_LOG(level, message, ...) clench::utils::doLogging(level, message, __VA_ARGS__)

	#define CLENCH_FATAL_LOG(message, ...) CLENCH_LOG(clench::utils::LogLevel::Error, message, __VA_ARGS__)
	#define CLENCH_ERROR_LOG(message, ...) CLENCH_LOG(clench::utils::LogLevel::Error, message, __VA_ARGS__)
	#define CLENCH_WARNING_LOG(message, ...) CLENCH_LOG(clench::utils::LogLevel::Warning, message, __VA_ARGS__)
	#define CLENCH_NOTE_LOG(message, ...) CLENCH_LOG(clench::utils::LogLevel::Note, message, __VA_ARGS__)
	#define CLENCH_INFO_LOG(message, ...) CLENCH_LOG(clench::utils::LogLevel::Info, message, __VA_ARGS__)
	#define CLENCH_DEBUG_LOG(message, ...) CLENCH_LOG(clench::utils::LogLevel::Debug, message, __VA_ARGS__)

#endif

#endif
