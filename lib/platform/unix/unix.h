/* ===========================================================================
    Information:
        lib/platform/unix.h   0.1.0 2024/04/28 <claymore>
    Description:
        thin wrapper around unix specific io operations
   ========================================================================= */

#ifndef FD_PLATFORM_UNIX
#define FD_PLATFORM_UNIX
#pragma once

#include "platform/os.h"
#include "utils/defs.h"

FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        fd_unix_stat_file
    Purpose:
        populates the 'fd_file_info' structure
    Arguments:
        | type         | name        | description |
        | ------------ | ----------- | ----------- |
        | char const * | path        | filename    |
   ------------------------------------------------------------------------- */
FD_EXPORT struct fd_file_info fd_unix_stat_file(char const *path);

FD_EXTERN_END

#endif /* !FD_PLATFORM_UNIX */
