#ifndef _CLENCH_PLAT_BASEDEFS_H_
#define _CLENCH_PLAT_BASEDEFS_H_

#include <clench/base/basedefs.h>

#if CLCMOD_BUILDING
#define CLCMOD_API DLLEXPORT
#else
#define CLCMOD_API DLLIMPORT
#endif

#endif
