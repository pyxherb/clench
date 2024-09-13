#ifndef _CLENCH_WSAL_BASEDEFS_H_
#define _CLENCH_WSAL_BASEDEFS_H_

#include <clench/base/attribs.h>
#include <clench/base/clsmacro.h>

#ifdef CLCWSAL_BUILDING
#define CLCWSAL_API DLLEXPORT
#else
#define CLCWSAL_API DLLIMPORT
#endif

#endif
