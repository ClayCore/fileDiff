/* ===========================================================================
    Information:
        @(#)platform/win32/file.c   0.1.0 2024/04/28
        lib/platform/win32/file.c   0.1.0 <claymore>
    Description:
        win32 file operations and processing
   ========================================================================= */

#include <stdlib.h>
#include <string.h>

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif /* WIN32_LEAN_AND_MEAN */

#ifndef _UNICODE
    #define _UNICODE
#endif /* _UNICODE */
#ifndef UNICODE
    #define UNICODE
#endif /* UNICODE */

#include <windows.h>


#include "file.h"
#include "logging/log.h"
#include "utils/defs.h"


struct fdlib_file_info fdlib_win32_stat_file(char const *path)
{
    struct fdlib_file_info fi = { 0 };

    char out[MAX_PATH] = { 0 };
    char *out_ptr      = out;
    char **part        = NULL;

    GetFullPathNameA(path, MAX_PATH, out_ptr, part);

    FD_LOG_INFO("in_path: \'%s\'", path);
    FD_LOG_INFO("out_path: \'%s\'", out_ptr);

    size_t out_path_len = strlen(out);
    fi.namelen          = out_path_len;

    if (part != NULL) {
        FD_LOG_INFO("part_path: \'%s\'", *part);
    }

    fi.name = malloc(out_path_len + 1);
    if (!fi.name) {
        FD_LOG_ERROR("failed to alloc \'name\'");
        abort();
    }

    memcpy(fi.name, out_ptr, out_path_len);
    fi.name[out_path_len] = '\0';

    return fi;
}
