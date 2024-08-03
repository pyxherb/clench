#ifndef _CLENCH_UTILS_UUID_H_
#define _CLENCH_UTILS_UUID_H_

#include "basedefs.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <cstring>

#define CLENCH_UUID(a, b, c, d, e) \
	{ 0x##a, 0x##b, 0x##c, 0x##d, 0x##e }

namespace clench {
	namespace utils {
		class UUIDParsingError : public std::runtime_error {
		public:
			inline UUIDParsingError() : std::runtime_error("Error parsing UUID") {}
			virtual ~UUIDParsingError() = default;
		};

		struct UUID {
			uint32_t a;
			uint16_t b, c, d;
			uint64_t e;

			/// @brief Parse a UUID from a string.
			/// @param s String to be parsed.
			/// @throws UUIDParsingError
			CLCUTILS_API static bool from(const char *s, UUID &uuidOut);

			inline bool operator<(const UUID &other) const {
				return memcmp(this, &other, sizeof(UUID)) < 0;
			}

			inline bool operator>(const UUID &other) const {
				return memcmp(this, &other, sizeof(UUID)) > 0;
			}

			inline bool operator==(const UUID &other) const {
				return !memcmp((void *)this, (void *)&other, sizeof(UUID));
			}
		};

		CLCUTILS_API void generateRandomUUID(UUID &uuidOut);
	}
}

namespace std {
	std::string to_string(const clench::utils::UUID &uuid);
}

#endif
