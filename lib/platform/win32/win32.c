/* ===========================================================================
    Information:
        @(#)platform/win32.c   0.1.0 2024/04/28
        lib/platform/win32.c   0.1.0 <claymore>
    Description:
        thin wrapper around win32 specific io operations
   ========================================================================= */

#include "win32.h"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif /* WIN32_LEAN_AND_MEAN */

#ifndef _UNICODE
    #define _UNICODE
#endif /* _UNICODE */

#include <windows.h>

#include <stdlib.h>

FD_EXPORT struct fdlib_file_info fdlib_stat_file(usize pathlen, char const *path)
{
    struct fdlib_file_info fi = { 0 };

    return fi;
}
