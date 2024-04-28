/* ===========================================================================
    Information:
        @(#)utils/defs.h   0.1.0 2024/04/28
        lib/utils/defs.h   0.1.0 <claymore>
    Description:
        preprocessor macros
   ========================================================================= */

#ifndef FD_LIB_DEFS
#define FD_LIB_DEFS
#pragma once

#include "FileDiff.h"

#define FD_STRX(x) #x
#define FD_STR(x)  FD_STRX(x)

#ifdef __cplusplus
    #define FD_EXTERN_BEGIN extern "C" {
    #define FD_EXTERN_END   }
#else
    #define FD_EXTERN_BEGIN
    #define FD_EXTERN_END
#endif /* __cplusplus */

#ifndef local
    #define local static
#endif /* local */

#ifndef internal
    #define internal static
#endif /* internal */

#ifndef global
    #define global static
#endif /* global */

#endif /* FD_LIB_DEFS */
