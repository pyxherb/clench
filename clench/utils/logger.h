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

		typedef void (*Logger)(LogLevel level, const char *source, const char *message, va_list varArgs);

		CLCUTILS_API void defaultLogger(LogLevel level, const char *source, const char *message, va_list varArgs);

		extern Logger g_logger;

		CLCUTILS_API void doLogging(LogLevel level, const char *source, const char *message, ...);
	}
}

#ifdef NDEBUG

	#define CLENCH_LOG(level, message, ...)

#else

	#define CLENCH_LOG(level, source, message, ...) clench::utils::doLogging(level, source, message, __VA_ARGS__)

	#define CLENCH_FATAL_LOG(source, message, ...) CLENCH_LOG(clench::utils::LogLevel::Error, source, message, __VA_ARGS__)
	#define CLENCH_ERROR_LOG(source, message, ...) CLENCH_LOG(clench::utils::LogLevel::Error, source, message, __VA_ARGS__)
	#define CLENCH_WARNING_LOG(source, message, ...) CLENCH_LOG(clench::utils::LogLevel::Warning, source, message, __VA_ARGS__)
	#define CLENCH_NOTE_LOG(source, message, ...) CLENCH_LOG(clench::utils::LogLevel::Note, source, message, __VA_ARGS__)
	#define CLENCH_INFO_LOG(source, message, ...) CLENCH_LOG(clench::utils::LogLevel::Info, source, message, __VA_ARGS__)
	#define CLENCH_DEBUG_LOG(source, message, ...) CLENCH_LOG(clench::utils::LogLevel::Debug, source, message, __VA_ARGS__)

#endif

#endif
