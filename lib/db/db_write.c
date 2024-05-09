/* ===========================================================================
    Information:
        @(#)db/db_write.c   0.1.0 2024/05/09
        lib/db/db_write.c   0.1.0 <claymore>
    Description:
        database write operations
   ========================================================================= */

#include "db.h"
#include "logging/log.h"

#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------------
    diagnostic ignore rationale:
        - '$' in identifiers has been supported in 3 major compiler
            families for ages now.
        - used as a means to say "this is private" without
            prepending the '_' character, which is possibly UB
 --------------------------------------------------------------------------- */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

FD_INTERNAL int db$write_generic(void **handle, void const *user, size_t size)
{
    int retval = FD_DBERR_SUCCESS;

    size_t count = 0;
    uint8_t *buf = NULL;

    if (!(*handle)) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(handle));

        return retval;
    }

    buf = malloc(size);
    if (!buf) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for buffer", fd_db_strerror(retval));

        return retval;
    }

    memcpy(buf, user, size);

    count = fwrite(buf, size, 1, *handle);
    if (count != 1) {
        retval = FD_DBERR_WRITE;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    free(buf);

    return retval;
}

FD_NO_EXPORT int db$write_header(void **handle, struct fd_db_header const *data)
{
    return db$write_generic(handle, data, sizeof *data);
}

FD_NO_EXPORT int db$write_config(void **handle, struct fd_db_config const *data)
{
    return db$write_generic(handle, data, sizeof *data);
}

FD_NO_EXPORT int db$write_dir(void **handle, struct fd_db_dir const *data)
{
    return db$write_generic(handle, data, sizeof *data);
}

FD_NO_EXPORT int db$write_file(void **handle, struct fd_db_file const *data)
{
    return db$write_generic(handle, data, sizeof *data);
}

#pragma clang diagnostic pop
