/* ===========================================================================
    Information:
        lib/platform/win32/file.h   0.1.0 2024/04/28 <claymore>
    Description:
        win32 file operations and processing
   ========================================================================= */

#ifndef FD_PLATFORM_WIN32_FILE
#define FD_PLATFORM_WIN32_FILE
#pragma once

#include "utils/defs.h"
#include "win32.h"

FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        fd_win32_stat_file
    Purpose:
        populates the 'fd_file_info' structure
    Arguments:
        | type         | name | description       |
        | ------------ | ---- | ----------------- |
        | char const * | path | path to the file  |
    Return value:
        | type         | description              |
        | ------------ | ------------------------ |
        | fd_file_info | populated info structure |
   ------------------------------------------------------------------------- */
FD_EXPORT struct fd_file_info fd_win32_stat_file(char const *path);

FD_EXTERN_END

#endif /* !FD_PLATFORM_WIN32_FILE */
