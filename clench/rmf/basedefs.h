#ifndef _CLENCH_PLAT_BASEDEFS_H_
#define _CLENCH_PLAT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCRMF_BUILDING
		#define CLCRMF_API DLLEXPORT
	#else
		#define CLCRMF_API DLLIMPORT
	#endif
#else
	#define CLCRMF_API
#endif

#endif
