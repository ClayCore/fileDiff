/* ===========================================================================
    Information:
        @(#)vfs/file.h   0.1.0 2024/04/28
        lib/vfs/file.h   0.1.0 <claymore>
    Description:
        file record metadata for the database
   ========================================================================= */

#ifndef FD_VFS_FILE
#define FD_VFS_FILE
#pragma once

#include "types/aliases.h"
#include "utils/defs.h"

FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        fdlib_file
    Description:
        TODO
   ------------------------------------------------------------------------- */
struct fdlib_file
{
    char const *path;
    usize pathlen;
};

LIBFILEDIFF_EXPORT struct fdlib_file *fdlib_file_new(usize, char const *);
LIBFILEDIFF_EXPORT void fdlib_file_destroy(struct fdlib_file *);

FD_EXTERN_END

#endif /* FD_VFS_FILE */
