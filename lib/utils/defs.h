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

#include "libFileDiffExport.h"  // IWYU pragma: export

#include <inttypes.h>  // IWYU pragma: keep
#include <stdbool.h>   // IWYU pragma: keep
#include <stddef.h>    // IWYU pragma: keep
#include <stdint.h>    // IWYU pragma: keep

#define FD_STRX(x) #x
#define FD_STR(x)  FD_STRX(x)

#ifdef __cplusplus
    #define FD_EXTERN_BEGIN extern "C" {
    #define FD_EXTERN_END   }
#else
    #define FD_EXTERN_BEGIN
    #define FD_EXTERN_END
#endif /* __cplusplus */

#ifndef FD_LOCAL
    #define FD_LOCAL static
#endif /* FD_LOCAL */

#ifndef FD_INTERNAL
    #define FD_INTERNAL static
#endif /* FD_INTERNAL */

#ifndef FD_GLOBAL
    #define FD_GLOBAL static
#endif /* FD_GLOBAL */

#endif /* FD_LIB_DEFS */
