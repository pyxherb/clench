#include "logger.h"
#include <cstdio>

using namespace clench;
using namespace clench::utils;

Logger clench::utils::g_logger = defaultLogger;

CLCUTILS_API void clench::utils::defaultLogger(LogLevel level, const char *source, const char *message, va_list varArgs) {
	switch (level) {
		case LogLevel::Fatal:
			fputs("[Fatal]", stdout);
			break;
		case LogLevel::Error:
			fputs("[Error]", stdout);
			break;
		case LogLevel::Warning:
			fputs("[Warning]", stdout);
			break;
		case LogLevel::Note:
			fputs("[Note]", stdout);
			break;
		case LogLevel::Info:
			fputs("[Info]", stdout);
			break;
		case LogLevel::Debug:
			fputs("[Debug]", stdout);
			break;
		default:
			break;
	}
	printf("(%s)", source);

	vprintf(message, varArgs);

	putchar('\n');
}

CLCUTILS_API void clench::utils::doLogging(LogLevel level, const char *source, const char *message, ...) {
	va_list ap;

	va_start(ap, message);

	if (g_logger)
		g_logger(level, source, message, ap);

	va_end(ap);
}
