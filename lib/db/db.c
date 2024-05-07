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

extern int _db_read_generic(void **, void *, size_t);
extern int _db_read_header(void **stream, struct fd_db_header *h);
extern int _db_read_config(void **stream, struct fd_db_config *c);
extern int _db_read_dir_entry(void **stream, struct fd_db_direntry *de);
extern int _db_read_entry(void **stream, struct fd_db_entry *e);


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
    int retval      = FD_DBERR_SUCCESS;
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
    int retval = FD_DBERR_SUCCESS;

    if (!(*stream)) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(stream));

        return retval;
    }

    fclose(*stream);

    return retval;
}

int fd_db_read(void **s)
{
    int retval = FD_DBERR_SUCCESS;

    struct fd_db_header *head         = NULL;
    struct fd_db_config *config       = NULL;
    struct fd_db_direntry *direntries = NULL;
    struct fd_db_entry *entries       = NULL;

    size_t head_size      = sizeof(head);
    size_t config_size    = sizeof(config);
    size_t dir_entry_size = sizeof *direntries;
    size_t entry_size     = sizeof *entries;

    head = malloc(head_size);
    if ((retval = _db_read_header(s, head)) != FD_DBERR_SUCCESS)
        return retval;

    config = malloc(config_size);
    if ((retval = _db_read_config(s, config)) != FD_DBERR_SUCCESS)
        return retval;

    direntries = calloc(head->dir_entries, dir_entry_size);
    for (size_t i = 0; i < head->dir_entries; ++i) {
        if ((retval = _db_read_dir_entry(s, &direntries[i])) != FD_DBERR_SUCCESS)
            return retval;
    }

    entries = calloc(head->entries, entry_size);
    for (size_t i = 0; i < head->entries; ++i) {
        if ((retval = _db_read_entry(s, &entries[i])) != FD_DBERR_SUCCESS)
            return retval;
    }

    return retval;
}

int fd_db_write(void **stream)
{
    int retval = FD_DBERR_SUCCESS;

    return retval;
}
