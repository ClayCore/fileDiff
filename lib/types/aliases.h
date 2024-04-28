/* ===========================================================================
    Information:
        @(#)types/aliases.h   0.1.0 2024/04/28
        lib/types/aliases.h   0.1.0 <claymore>
    Description:
        preprocessor macros
   ========================================================================= */

#ifndef FD_TYPES_ALIASES
#define FD_TYPES_ALIASES
#pragma once

#include <inttypes.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uintptr_t usize;
typedef ptrdiff_t isize;

typedef uintptr_t uptr;
typedef intptr_t iptr;

typedef uint_fast32_t b32;

typedef float_t f32;
typedef double_t f64;
typedef long double f128;

#endif /* FD_TYPES_ALIASES */
