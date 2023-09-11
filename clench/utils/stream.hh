#ifndef _CLENCH_UTILS_STREAM_HH_
#define _CLENCH_UTILS_STREAM_HH_

#include <cstdio>
#include <iostream>

namespace clench {
	namespace utils {
		inline void* readAll(std::istream& is, std::size_t& destSize) {
			is.seekg(std::ios::end);
			destSize = is.tellg();
			is.seekg(std::ios::beg);

			char* buf = new char[destSize];
			is.read(buf, destSize);

			return buf;
		}

		inline char* readAll(std::FILE* fp, std::size_t& destSize) {
			std::fseek(fp, 0, SEEK_END);
			destSize = ftell(fp);
			if (destSize < 0)
				throw std::runtime_error("Error seeking file");
			std::fseek(fp, 0, SEEK_SET);

			char* buf = new char[destSize];
			if (!fread(buf, destSize, 1, fp)) {
				delete[] buf;
				throw std::runtime_error("Error reading file");
			}

			return buf;
		}
	}
}

#endif
