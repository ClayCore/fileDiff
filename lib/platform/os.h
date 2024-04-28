/* ===========================================================================
    Information:
        @(#)platform/os.h   0.1.0 2024/04/28
        lib/platform/os.h   0.1.0 <claymore>
    Description:
        includes specific headers based on platform
   ========================================================================= */

#ifndef FD_PLATFORM_OS
#define FD_PLATFORM_OS
#pragma once

#include "types/aliases.h"
#include "utils/defs.h"

FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        fdlib_file_info
    Description:
        platform-agnostic file information structure

        timestamps are stored as 64 bit unsigned integers
    Fields:
        | type         | name        | description                 |
        | ------------ | ----------- | --------------------------- |
        | u64          | create_time | file creation timestamp     |
        | u64          | access_time | file access timestamp       |
        | u64          | mod_time    | file modification timestamp |
        | usize        | namelen     | length of the filename      |
        | char const * | name        | filename                    |
   ------------------------------------------------------------------------- */
struct fdlib_file_info
{
    u64 access_time;
    u64 mod_time;
    usize namelen;
    char const *name;
};

#ifdef DEV_WIN32
    #include "win32/win32.h"  // IWYU pragma: export
#else                         // Assuming unix/linux
    #include "unix/unix.h"    // IWYU pragma: export
#endif

FD_EXTERN_END

#endif /* FD_PLATFORM_OS */
