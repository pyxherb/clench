#ifndef _CLENCH_BASE_BASEDEFS_H_
#define _CLENCH_BASE_BASEDEFS_H_

#include <clench/base/attribs.h>
#include <clench/base/clsmacro.h>

#if CLENCH_DYNAMIC_LINK
	#ifdef CLCBASE_BUILDING
		#define CLCBASE_API DLLEXPORT
	#else
		#define CLCBASE_API DLLIMPORT
	#endif
#else
	#define CLCBASE_API
#endif

#endif
