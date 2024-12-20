#ifndef _CLENCH_PLAT_BASEDEFS_H_
#define _CLENCH_PLAT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCTCM_BUILDING
		#define CLCTCM_API CLENCH_DLLEXPORT
	#else
		#define CLCTCM_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCTCM_API
#endif

#endif
