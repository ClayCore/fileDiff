/* ===========================================================================
    Information:
        lib/db/db_read.c   0.1.0 2024/05/07 <claymore>
    Description:
        database read operations
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

/* ---------------------------------------------------------------------------
    Name:
        db$read_generic
    Purpose:
        reads a byte array from the database file
    Arguments:
        | type    | name   | description                     |
        | ------- | ------ | ------------------------------- |
        | void ** | handle | owned pointer to database file  |
        | void *  | user   | out param, bytes that were read |
        | size_t  | size   | how many bytes to read          |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
FD_INTERNAL int db$read_generic(void **handle, void *user, size_t size)
{
    int retval = FD_DBERR_SUCCESS;
    FILE *fp   = *handle;

    size_t count = 0;
    uint8_t *buf = NULL;


    if (fp == NULL) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(fp));

        return retval;
    }

    buf = malloc(size);
    if (!buf) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for buffer", fd_db_strerror(retval));

        return retval;
    }

    count = fread(buf, size, 1, fp);
    if (count != 1) {
        retval = FD_DBERR_READ;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    memcpy(user, buf, size);

    free(buf);


    return retval;
}

FD_NO_EXPORT int db$read_header(void **handle, struct fd_db_header *data)
{
    return db$read_generic(handle, data, sizeof *data);
}

FD_NO_EXPORT int db$read_config(void **handle, struct fd_db_config *data)
{
    return db$read_generic(handle, data, sizeof *data);
}

FD_NO_EXPORT int db$read_dir(void **handle, struct fd_db_dir *data)
{
    return db$read_generic(handle, data, sizeof *data);
}

FD_NO_EXPORT int db$read_file(void **handle, struct fd_db_file *data)
{
    return db$read_generic(handle, data, sizeof *data);
}

#pragma clang diagnostic pop
