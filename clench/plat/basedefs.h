#ifndef _CLENCH_PLAT_BASEDEFS_H_
#define _CLENCH_PLAT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCPLAT_BUILDING
		#define CLCPLAT_API CLENCH_DLLEXPORT
	#else
		#define CLCPLAT_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCPLAT_API
#endif

#endif
