#ifndef _CLENCH_PLAT_BASEDEFS_H_
#define _CLENCH_PLAT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCRMF_BUILDING
		#define CLCRMF_API CLENCH_DLLEXPORT
	#else
		#define CLCRMF_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCRMF_API
#endif

#endif
