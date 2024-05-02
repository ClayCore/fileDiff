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

#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif /* !_CRT_SECURE_NO_WARNINGS */

#include "libFileDiffExport.h"  // IWYU pragma: export

#include <inttypes.h>  // IWYU pragma: keep
#include <stdbool.h>   // IWYU pragma: keep
#include <stddef.h>    // IWYU pragma: keep
#include <stdint.h>    // IWYU pragma: keep
#include <string.h>    // IWYU pragma: keep

#define FD_STRX(x) #x
#define FD_STR(x)  FD_STRX(x)

#ifdef __cplusplus
    #define FD_EXTERN_BEGIN extern "C" {
    #define FD_EXTERN_END   }
#else /* __cplusplus */
    #define FD_EXTERN_BEGIN
    #define FD_EXTERN_END
#endif /* !__cplusplus */

#define FD_BUILD_ASSERT_OR_ZERO(cond) (sizeof(char[1 - 2 * !(cond)]) - 1)

// clang-format off
#define FD_SWAP(a, b)                                                   \
    do {                                                                \
        void *_swap_ap = &(a);                                          \
        void *_swap_bp = &(b);                                          \
        uint8_t *_swap_buf[sizeof(a)];                                  \
        memcpy(_swap_buf, _swap_ap, sizeof(a));                         \
        memcpy(                                                         \
            _swap_ap, _swap_bp,                                         \
            sizeof(a) + FD_BUILD_ASSERT_OR_ZERO(sizeof(a == sizeof(b))) \
        );                                                              \
        memcpy(_swap_bp, _swap_buf, sizeof(a));                         \
    } while (0)
// clang-format on

#define FD_BITSIZEOF(x)         (CHAR_BIT * sizeof(x))

#define FD_MAX_SIGNED_VAL(a)    (INTMAX_MAX >> (FD_BITSIZEOF(intmax_t) - FD_BITSIZEOF(a)))
#define FD_MAX_UNSIGNED_VAL(a)  (UINTMAX_MAX >> (FD_BITSIZEOF(uintmax_t) - FD_BITSIZEOF(a)))

#define FD_SIGNED_ADDOF(a, b)   ((b > FD_MAX_SIGNED_VAL(a) - (a)))
#define FD_UNSIGNED_ADDOF(a, b) ((b > FD_MAX_UNSIGNED_VAL(a) - (a)))

#define FD_LOCAL                static
#define FD_GLOBAL               static
#define FD_INTERNAL             static inline

#endif /* !FD_LIB_DEFS */
