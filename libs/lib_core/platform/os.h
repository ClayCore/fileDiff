/* ===========================================================================
    Information:
        lib/platform/os.h   0.1.0 2024/04/28 <claymore>
    Description:
        includes specific headers based on platform
   ========================================================================= */

#ifndef FD_PLATFORM_OS
#define FD_PLATFORM_OS
#pragma once

#include "utils/defs.h"

FD_EXTERN_BEGIN


/* ---------------------------------------------------------------------------
    Name:
        fdlib_file_info
    Description:
        platform-agnostic file information structure

        timestamps are stored as 64 bit unsigned integers
    Fields:
        | type    | name        | description                 |
        | ------- | ----------- | --------------------------- |
        | int64_t | create_time | file creation timestamp     |
        | int64_t | access_time | file access timestamp       |
        | int64_t | mod_time    | file modification timestamp |
        | size_t  | size        | size of the file            |
        | size_t  | namelen     | length of the filename      |
        | char *  | name        | filename                    |
   ------------------------------------------------------------------------- */
struct fd_file_info
{
    int64_t create_time;
    int64_t access_time;
    int64_t mod_time;
    size_t size;
    size_t namelen;
    char *name;
};


struct fd_file_ops
{
    bool (*compare)(struct fd_file_info const *, struct fd_file_info const *);
    char *(*hash)(struct fd_file_info const *);
};


#ifdef DEV_WIN32

    #include "win32/file.h"  // IWYU pragma: export
    #include "win32/path.h"  // IWYU pragma: export

    #define fd_stat_file(p)      (fd_win32_stat_file(p))
    #define fd_normalize_path(p) (fd_win32_normalize_path(p))
#else /* !DEV_WIN32 */

    #include "unix/unix.h"  // IWYU pragma: export

    #define fd_stat_file(p) (fd_unix_stat_file(p))
#endif /* DEV_WIN32 */

FD_EXTERN_END

#endif /* !FD_PLATFORM_OS */
