#ifndef _CLENCH_PLAT_UUID_H_
#define _CLENCH_PLAT_UUID_H_

#include "basedefs.h"
#include <clench/utils/uuid.h>

namespace clench {
	namespace plat {
		CLCPLAT_API void generateUUID(base::UUID &uuidOut);
	}
}

#endif
