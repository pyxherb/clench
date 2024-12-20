#ifndef _CLENCH_GHAL_BASEDEFS_H_
#define _CLENCH_GHAL_BASEDEFS_H_

#include <clench/base/basedefs.h>
#include <clench/base/clsmacro.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCGHAL_BUILDING
		#define CLCGHAL_API CLENCH_DLLEXPORT
	#else
		#define CLCGHAL_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCGHAL_API
#endif

#endif
