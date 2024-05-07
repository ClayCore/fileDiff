/* ===========================================================================
    Information:
        @(#)db/db.c   0.1.0 2024/05/04
        lib/db/db.c   0.1.0 <claymore>
    Description:
        database index for fetched files and paths
   ========================================================================= */

#include "db.h"
#include "logging/log.h"

#include <stdlib.h>
#include <string.h>


char const *FD_DB_ERROR_STR[] = {
    [FD_DBERR_NOFILE]   = "file not found",             //
    [FD_DBERR_ALLOC]    = "failed to allocate memory",  //
    [FD_DBERR_READ]     = "read failed",                //
    [FD_DBERR_WRITE]    = "write failed",               //
    [FD_DBERR_MINVALID] = "invalid mode",               //
    [FD_DBERR_HANDLE]   = "handle is null",             //
    [FD_DBERR_MAGIC]    = "invalid magic number",       //
};

uint8_t const FD_DB_MAGIC[FD_DB_MAGIC_SIZE] = {
    'f', 'd', '_', 'm', 'g', 'k', '1', '\0',
};


char const *fd_db_strerror(int code)
{
    return FD_DB_ERROR_STR[code];
}

int fd_db_open(void **stream, int mode)
{
    int retval      = 0;
    errno_t errcode = 0;

    switch (mode) {
    case FD_DBOM_READ: {
        errcode = fopen_s(*stream, "private_data/db.dat", "rb+");
    } break;
    case FD_DBOM_WRITE: {
        errcode = fopen_s(*stream, "private_data/db.dat", "wb+");
    } break;
    default: {
        retval = FD_DBERR_MINVALID;
        FD_LOG_ERROR("%s: %d", fd_db_strerror(retval), mode);
        return retval;
    } break;
    }

    if (errcode != 0) {
        retval = FD_DBERR_NOFILE;
        FD_LOG_ERROR("%s: \'private_data/db.dat\'", fd_db_strerror(retval));
        return retval;
    }

    return retval;
}

int fd_db_close(void **stream)
{
    int retval = 0;

    if (!(*stream)) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(stream));

        return retval;
    }

    fclose(*stream);

    return retval;
}

int fd_db_read(void **stream)
{
    int retval   = 0;
    size_t terr  = 0;
    uint8_t *buf = NULL;

    struct fd_db_header head;
    size_t head_size = sizeof(head);

    struct fd_db_config config;
    size_t config_size = sizeof(config);

    struct fd_db_direntry *dir_entries;
    size_t dir_entry_size = sizeof *dir_entries;

    struct fd_db_entry *entries;
    size_t entry_size = sizeof *entries;


    if (!(*stream)) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(stream));

        return retval;
    }

    buf = malloc(head_size);
    if (!buf) {
        retval = FD_DBERR_ALLOC;

        goto error;
    }

    terr = fread(buf, head_size, 1, *stream);
    if (terr != 1) {
        retval = FD_DBERR_READ;

        goto error;
    }

    memcpy(&head, buf, head_size);


    for (int i = 0; i < FD_DB_MAGIC_SIZE; ++i) {
        if (head.magic[i] != FD_DB_MAGIC[i]) {
            retval = FD_DBERR_MAGIC;

            goto error;
        }
    }

    buf = realloc(buf, config_size);
    if (!buf) {
        retval = FD_DBERR_ALLOC;

        goto error;
    }

    terr = fread(buf, config_size, 1, *stream);
    if (terr != 1) {
        retval = FD_DBERR_READ;

        goto error;
    }

    memcpy(&config, buf, config_size);


    buf = realloc(buf, dir_entry_size * head.dir_entries);
    if (!buf) {
        retval = FD_DBERR_ALLOC;

        goto error;
    }

    terr = fread(buf, dir_entry_size, head.dir_entries, *stream);
    if (terr != head.dir_entries) {
        retval = FD_DBERR_READ;

        goto error;
    }

    dir_entries = calloc(head.dir_entries, dir_entry_size);
    if (!dir_entries) {
        retval = FD_DBERR_ALLOC;

        goto error;
    }

    for (size_t i = 0; i < head.dir_entries; ++i) {
        memcpy(&dir_entries[i], buf + (i * dir_entry_size), dir_entry_size);
    }


    buf = realloc(buf, entry_size * head.entries);
    if (!buf) {
        retval = FD_DBERR_ALLOC;

        goto error;
    }

    terr = fread(buf, entry_size, head.entries, *stream);
    if (terr != head.entries) {
        retval = FD_DBERR_READ;

        goto error;
    }

    entries = calloc(head.entries, entry_size);
    if (!entries) {
        retval = FD_DBERR_ALLOC;

        goto error;
    }

    for (size_t i = 0; i < head.entries; ++i) {
        memcpy(&entries[i], buf + (i * entry_size), entry_size);
    }

    return retval;

error:
    FD_LOG_ERROR("%s", fd_db_strerror(retval));

    return retval;
}

int fd_db_write(void **stream)
{
    int retval = 0;

    return retval;
}
