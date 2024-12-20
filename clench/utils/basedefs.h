#ifndef _CLENCH_UTILS_BASEDEFS_H_
#define _CLENCH_UTILS_BASEDEFS_H_

#include <clench/base/attribs.h>
#include <clench/base/clsmacro.h>

#if CLENCH_DYNAMIC_LINK
	#if CLCUTILS_BUILDING
		#define CLCUTILS_API CLENCH_DLLEXPORT
	#else
		#define CLCUTILS_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCUTILS_API
#endif

#endif
