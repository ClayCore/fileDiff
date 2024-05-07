/* ===========================================================================
    Information:
        @(#)db/db_read.c   0.1.0 2024/05/07
        lib/db/db_read.c   0.1.0 <claymore>
    Description:
        database read operations
   ========================================================================= */

#include "db.h"
#include "libFileDiffExport.h"
#include "logging/log.h"

#include <stdlib.h>
#include <string.h>

FD_INTERNAL int _db_read_generic(void **stream, void *user, size_t size)
{
    int retval   = FD_DBERR_SUCCESS;
    size_t count = 0;

    uint8_t *buf = NULL;

    if (!(*stream)) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(stream));

        return retval;
    }

    buf = malloc(size);
    if (!buf) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    count = fread(buf, size, 1, *stream);
    if (count != 1) {
        retval = FD_DBERR_READ;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    memcpy(user, buf, size);

    return retval;
}

FD_NO_EXPORT int _db_read_header(void **stream, struct fd_db_header *h)
{
    return _db_read_generic(stream, h, sizeof *h);
}

FD_NO_EXPORT int _db_read_config(void **stream, struct fd_db_config *c)
{
    return _db_read_generic(stream, c, sizeof *c);
}

FD_NO_EXPORT int _db_read_dir_entry(void **stream, struct fd_db_direntry *de)
{
    return _db_read_generic(stream, de, sizeof *de);
}

FD_NO_EXPORT int _db_read_entry(void **stream, struct fd_db_entry *e)
{
    return _db_read_generic(stream, e, sizeof *e);
}
