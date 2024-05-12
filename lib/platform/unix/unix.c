/* ===========================================================================
    Information:
        lib/platform/unix.c   0.1.0 2024/04/28 <claymore>
    Description:
        thin wrapper around unix specific io operations
   ========================================================================= */

#ifndef DEV_WIN32

    #include "unix.h"
    #include "logging/log.h"
    #include "utils/defs.h"


    #include <sys/stat.h>
    #include <unistd.h>


    #include <stdlib.h>
    #include <string.h>


    #define FD_UNIX_MAXPATH (1024)

FD_EXPORT struct fd_file_info fd_unix_stat_file(char const *path)
{
    struct fdlib_file_info fi      = { 0 };
    struct stat filestat           = { 0 };
    char out_path[FD_UNIX_MAXPATH] = { 0 };
    char *out_ptr                  = out_path;

    out_ptr = realpath(path, out_ptr);

    int32_t err = stat(out_ptr, &filestat);
    if (err != 0) {
        FD_LOG_ERROR("failed to stat \'%s\'", path);
        abort();
    }

    fi.name    = out_ptr;
    fi.namelen = strlen(out_ptr);

    fi.access_time = filestat.st_atime;
    fi.create_time = filestat.st_ctime;
    fi.mod_time    = filestat.st_mtime;

    return fi;
}

#endif /* DEV_WIN32 */
