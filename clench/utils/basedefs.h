#ifndef _CLENCH_UTILS_BASEDEFS_H_
#define _CLENCH_UTILS_BASEDEFS_H_

#include <clench/base/attribs.h>
#include <clench/base/clsmacro.h>

#ifdef CLCUTILS_BUILDING
#define CLCUTILS_API DLLEXPORT
#else
#define CLCUTILS_API DLLIMPORT
#endif

#endif
