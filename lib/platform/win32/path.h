/* ===========================================================================
    Information:
        lib/platform/win32/path.h   0.1.0 2024/04/28 <claymore>
    Description:
        win32 path manipulation
   ========================================================================= */

#ifndef FD_PLATFORM_WIN32_PATH
#define FD_PLATFORM_WIN32_PATH
#pragma once

#include "utils/defs.h"
#include "win32.h"


FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        FD_WIN32_PATH_TYPE
    Values:
        | value                | description                        |
        | ==================== | ================================== |
        | FD_PT_ABSOLUTE       | absolute path ("C:\f")             |
        | -------------------- | ---------------------------------- |
        | FD_PT_INVALID        | any invalid path                   |
        | -------------------- | ---------------------------------- |
        | FD_PT_NETWORK        | network path ("\\server\f")        |
        | -------------------- | ---------------------------------- |
        | FD_PT_RELATIVE_DRIVE | relative to current drive ("\c\f") |
        | -------------------- | ---------------------------------- |
        | FD_PT_RELATIVE_ROOT  | relative to current root ("C:f\d") |
        | -------------------- | ---------------------------------- |
        | FT_PT_RELATIVE       | any valid path that doesn't        |
        |                      | fall into any other category       |
        | -------------------- | ---------------------------------- |
        | FT_PT_UNC            | unc path ("\\?\devices\f")         |
   ------------------------------------------------------------------------- */
enum FD_WIN32_PATH_TYPE
{
    FD_PT_ABSOLUTE,
    FD_PT_INVALID,
    FD_PT_NETWORK,
    FD_PT_RELATIVE_DRIVE,
    FD_PT_RELATIVE_ROOT,
    FD_PT_RELATIVE,
    FD_PT_UNC,
};

/* ---------------------------------------------------------------------------
    Name:
        fd_win32_normalize_path
    Purpose:
        normalizes the unnormalized path provided
        the path is a valid windows path
    Arguments:
        | type   | name | description       |
        | ------ | ---- | ----------------- |
        | char * | path | unnormalized path |
    Return value:
        | type   | description              |
        | ------ | ------------------------ |
        | char * | normalized path          |
   ------------------------------------------------------------------------- */
FD_EXPORT char *fd_win32_normalize_path(char *path);

FD_EXTERN_END

#endif /* !FD_PLATFORM_WIN32_PATH */
