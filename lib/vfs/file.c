/* ===========================================================================
    Information:
        @(#)vfs/file.c   0.1.0 2024/04/28
        lib/vfs/file.c   0.1.0 <claymore>
    Description:
        file record metadata for the database
   ========================================================================= */

#include "vfs/file.h"

#include <stdlib.h>


LIBFILEDIFF_EXPORT struct fdlib_file *fdlib_file_new(usize n, char const *path)
{
    struct fdlib_file *f = calloc(1, sizeof *f);

    f->path    = path;
    f->pathlen = n;

    return f;
}

LIBFILEDIFF_EXPORT void fdlib_file_destroy(struct fdlib_file *file)
{
    free(file);
    file = NULL;
}
