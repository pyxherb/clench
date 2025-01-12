#ifndef _CLENCH_UTILS_UUID_H_
#define _CLENCH_UTILS_UUID_H_

#include "basedefs.h"
#include <clench/base/uuid.h>
#include <string>

namespace clench {
	namespace utils {
		CLCUTILS_API bool parseUUID(const char *s, base::UUID &uuidOut);
	}
}

namespace std {
	std::string to_string(const clench::base::UUID &uuid);
}

#endif
