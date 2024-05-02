/* ===========================================================================
    Information:
        @(#)platform/win32.h   0.1.0 2024/04/28
        lib/platform/win32.h   0.1.0 <claymore>
    Description:
        thin wrapper around win32 specific io operations
   ========================================================================= */

#ifndef FD_PLATFORM_WIN32
#define FD_PLATFORM_WIN32
#pragma once

#include "platform/os.h"
#include "utils/defs.h"

FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        fdlib_win32_normalize_path
    Purpose:
        normalizes
    Arguments:
        | type   | name | description       |
        | ------ | ---- | ----------------- |
        | char * | path | unnormalized path |
    Return value:
        | type   | description     |
        | ------ | --------------- |
        | char * | normalized path |
   ------------------------------------------------------------------------- */
FD_EXPORT char *fdlib_win32_normalize_path(char *path);

/* ---------------------------------------------------------------------------
    Name:
        fdlib_win32_stat_file
    Purpose:
        populates the 'fdlib_file_info' structure
    Arguments:
        | type         | name | description      |
        | ------------ | ---- | ---------------- |
        | char const * | path | path to the file |
    Return value:
        | type          | description              |
        | ------------- | ------------------------ |
        | @(#)file_info | populated info structure |
   ------------------------------------------------------------------------- */
FD_EXPORT struct fdlib_file_info fdlib_win32_stat_file(char const *path);

FD_EXTERN_END

#endif /* !FD_PLATFORM_WIN32 */
