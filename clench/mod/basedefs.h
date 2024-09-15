#ifndef _CLENCH_PLAT_BASEDEFS_H_
#define _CLENCH_PLAT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCMOD_BUILDING
		#define CLCMOD_API DLLEXPORT
	#else
		#define CLCMOD_API DLLIMPORT
	#endif
#else
	#define CLCMOD_API DLLEXPORT
#endif

#endif
