#ifndef _CLENCH_DEBUG_LOG_HH_
#define _CLENCH_DEBUG_LOG_HH_

#include <cstdarg>
#include <cstdio>
#include <string>

#include "mm.hh"

#ifdef _WIN32

	#include <Windows.h>

#endif

namespace clench {
	namespace debug {
		void inline message(std::string fmt, std::va_list args) noexcept {
#ifndef NDEBUG
			std::vprintf(fmt.c_str(), args);
#endif
		}

		void inline message(std::string fmt, ...) noexcept {
#ifndef NDEBUG
			std::va_list args;
			va_start(args, fmt);
			message(fmt, args);
			va_end(args);
#endif
		}

		void inline message(std::string type, std::string fmt, std::va_list args) {
#ifndef NDEBUG
			message("[" + type + "] " + fmt + "\n", args);
#endif
		}

		void inline message(std::string type, std::string fmt, ...) {
#ifndef NDEBUG
			std::va_list args;
			va_start(args, fmt);
			message(type, fmt, args);
			va_end(args);
#endif
		}

		void inline info(std::string fmt, ...) {
#ifndef NDEBUG
			std::va_list args;
			va_start(args, fmt);
			message("INFO", fmt, args);
			va_end(args);
#endif
		}

		void inline warn(std::string fmt, ...) {
#ifndef NDEBUG
	#ifdef _WIN32
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	#endif

			std::va_list args;
			va_start(args, fmt);
			message("WARN", fmt, args);
			va_end(args);

	#ifdef _WIN32
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#endif
#endif
		}

		void inline note(std::string fmt, ...) {
#ifndef NDEBUG
	#ifdef _WIN32
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	#endif

			std::va_list args;
			va_start(args, fmt);
			message("NOTE", fmt, args);
			va_end(args);

	#ifdef _WIN32
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#endif
#endif
		}

		void inline error(std::string fmt, ...) {
#ifndef NDEBUG
	#ifdef _WIN32
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_INTENSITY);
	#endif
			std::va_list args;
			va_start(args, fmt);
			message("ERROR", fmt, args);
			va_end(args);

	#ifdef _WIN32
			SetConsoleTextAttribute(
				GetStdHandle(STD_OUTPUT_HANDLE),
				FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	#endif
#endif
		}
	}
}

#endif
