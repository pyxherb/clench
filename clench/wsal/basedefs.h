#ifndef _CLENCH_WSAL_BASEDEFS_H_
#define _CLENCH_WSAL_BASEDEFS_H_

#include <clench/base/attribs.h>
#include <clench/base/clsmacro.h>

#if CLENCH_DYNAMIC_LINK
	#ifdef CLCWSAL_BUILDING
		#define CLCWSAL_API CLENCH_DLLEXPORT
	#else
		#define CLCWSAL_API CLENCH_DLLIMPORT
	#endif
#else
	#define CLCWSAL_API
#endif

#endif
