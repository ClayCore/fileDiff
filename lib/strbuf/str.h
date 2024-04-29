/* ===========================================================================
    Information:
        @(#)strbuf/str.h   0.1.0 2024/04/29
        lib/strbuf/str.h   0.1.0 <claymore>
    Description:
        git-inspired non-null terminated string
    Notes:
        technically this interface needn't be exported
        because (hopefully) anything that interfaces
        with this library can use better alternatives
        on the ffi boundary i.e 'std::string' in C++
        or 'String/&str' in Rust

        nonetheless, since most likely a large part of
        this library is going to end up using this type
        internally, we might as well export it properly,
        so consumers can interface with everything
        that passes/returns the 'fdlib_str'
   ========================================================================= */

#ifndef FD_STRBUF_STR
#define FD_STRBUF_STR
#pragma once

#include "utils/defs.h"

FD_EXTERN_BEGIN

struct fdlib_str
{
    uint_least32_t alloc;
    uint_least32_t len;
    char *buf;
};

extern char fdlib_str_empty[];
#define FDLIB_STR_INIT         \
    (struct fdlib_str)         \
    {                          \
        .buf = fdlib_str_empty \
    }


FD_EXTERN_END

#endif /* !FD_STRBUF_STR */
