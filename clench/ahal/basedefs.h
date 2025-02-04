#ifndef _CLENCH_AHAL_BASEDEFS_H_
#define _CLENCH_AHAL_BASEDEFS_H_

#include <clench/base/basedefs.h>
#include <clench/base/clsmacro.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCAHAL_BUILDING
		#define CLCAHAL_API CLENCH_DLLEXPORT
	#else
		#define CLCAHAL_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCAHAL_API
#endif

#endif
