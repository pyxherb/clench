#ifndef _CLENCH_BASE_UUID_H_
#define _CLENCH_BASE_UUID_H_

#include "basedefs.h"

#include <cstdint>
#include <cstring>

namespace clench {
	namespace base {
		struct UUID {
			uint32_t a;
			uint16_t b, c, d;
			uint64_t e;

			CLENCH_FORCEINLINE bool operator<(const UUID &other) const {
				return memcmp(this, &other, sizeof(UUID)) < 0;
			}

			CLENCH_FORCEINLINE bool operator>(const UUID &other) const {
				return memcmp(this, &other, sizeof(UUID)) > 0;
			}

			CLENCH_FORCEINLINE bool operator==(const UUID &other) const {
				return !memcmp((void *)this, (void *)&other, sizeof(UUID));
			}
		};
	}
}

#define CLENCH_UUID(a, b, c, d, e) \
	clench::base::UUID { 0x##a, 0x##b, 0x##c, 0x##d, 0x##e }

#endif
