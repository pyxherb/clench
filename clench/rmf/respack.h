#ifndef _CLENCH_RMF_RESPACK_H_
#define _CLENCH_RMF_RESPACK_H_

#include "restype.h"

namespace clench {
	namespace rmf {
		struct ResourcePackEntry {
			std::string majorType;
			void *resource;
		};
		struct ResourcePack {
			std::unordered_map<std::string, ResourcePackEntry> entries;

			CLCRMF_API ~ResourcePack();
		};
	}
}

#endif
