#ifndef _CLENCH_GHAL_BASEDEFS_H_
#define _CLENCH_GHAL_BASEDEFS_H_

#include <clench/base/basedefs.h>
#include <clench/base/clsmacro.h>

#if CLCGHAL_BUILDING
	#define CLCGHAL_API DLLEXPORT
#else
	#define CLCGHAL_API DLLIMPORT
#endif

#endif
