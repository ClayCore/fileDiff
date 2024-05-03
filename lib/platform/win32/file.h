/* ===========================================================================
    Information:
        @(#)platform/win32/file.h   0.1.0 2024/04/28
        lib/platform/win32/file.h   0.1.0 <claymore>
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
        fdlib_win32_stat_file
    Purpose:
        populates the 'fdlib_file_info' structure
    Arguments:
        | type          | name | description       |
        | ------------- | ---- | ----------------- |
        | char const *  | path | path to the file  |
    Return value:
        | type          | description              |
        | ------------- | ------------------------ |
        | @(#)file_info | populated info structure |
   ------------------------------------------------------------------------- */
FD_EXPORT struct fdlib_file_info fdlib_win32_stat_file(char const *path);

FD_EXTERN_END

#endif /* !FD_PLATFORM_WIN32_FILE */
