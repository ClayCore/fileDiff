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
        db$read_header
    Purpose:
        read header structure from database file
    Arguments:
        | type           | name   | description                    |
        | -------------- | ------ | ------------------------------ |
        | void **        | handle | owned pointer to database file |
        | fd_db_header * | data   | out param, read header data    |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$read_header(void **handle, struct fd_db_header *data);

/* ---------------------------------------------------------------------------
    Name:
        db$read_config
    Purpose:
        read configuration from database file
    Arguments:
        | type           | name   | description                    |
        | -------------- | ------ | ------------------------------ |
        | void **        | handle | owned pointer to database file |
        | fd_db_config * | data   | out param, read config data    |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$read_config(void **handle, struct fd_db_config *data);

/* ---------------------------------------------------------------------------
    Name:
        db$read_dir
    Purpose:
        read dir entry from database file
    Arguments:
        | type        | name   | description                     |
        | ----------- | ------ | ------------------------------- |
        | void **     | handle | owned pointer to database file  |
        | fd_db_dir * | data   | out param, read directory entry |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$read_dir(void **handle, struct fd_db_dir *data);

/* ---------------------------------------------------------------------------
    Name:
        db$read_file
    Purpose:
        read file entry from database file
    Arguments:
        | type         | name   | description                    |
        | ------------ | ------ | ------------------------------ |
        | void **      | handle | owned pointer to database file |
        | fd_db_file * | data   | out param, read file entry     |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$read_file(void **handle, struct fd_db_file *data);

/* ---------------------------------------------------------------------------
    Name:
        db$write_header
    Purpose:
        write header structure to database
    Arguments:
        | type           | name   | description                    |
        | -------------- | ------ | ------------------------------ |
        | void **        | handle | owned pointer to database file |
        | fd_db_header * | data   | in param, header to write      |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$write_header(void **handle, struct fd_db_header const *data);

/* ---------------------------------------------------------------------------
    Name:
        db$write_config
    Purpose:
        write configuration to database
    Arguments:
        | type           | name   | description                    |
        | -------------- | ------ | ------------------------------ |
        | void **        | handle | owned pointer to database file |
        | fd_db_config * | data   | in param, config to write      |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$write_config(void **handle, struct fd_db_config const *data);

/* ---------------------------------------------------------------------------
    Name:
        db$write_dir
    Purpose:
        write directory entry to database
    Arguments:
        | type        | name   | description                        |
        | ----------- | ------ | ---------------------------------- |
        | void **     | handle | owned pointer to database file     |
        | fd_db_dir * | data   | in param, directory entry to write |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$write_dir(void **handle, struct fd_db_dir const *data);

/* ---------------------------------------------------------------------------
    Name:
        db$write_file
    Purpose:
        write file entry to database
    Arguments:
        | type         | name   | description                    |
        | ------------ | ------ | ------------------------------ |
        | void **      | handle | owned pointer to database file |
        | fd_db_file * | data   | in param, file entry to write  |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
extern int db$write_file(void **handle, struct fd_db_file const *data);

#pragma clang diagnostic pop


char const *FD_DB_ERROR_STR[] = {
    [FD_DBERR_SUCCESS]  = "success",                    //
    [FD_DBERR_NOFILE]   = "file not found",             //
    [FD_DBERR_ALLOC]    = "failed to allocate memory",  //
    [FD_DBERR_READ]     = "read failed",                //
    [FD_DBERR_WRITE]    = "write failed",               //
    [FD_DBERR_MINVALID] = "invalid mode",               //
    [FD_DBERR_HANDLE]   = "handle is null",             //
    [FD_DBERR_MAGIC]    = "invalid magic number",       //
    [FD_DBERR_NULLBUF]  = "buffer or data is null",     //
};

uint8_t const FD_DB_MAGIC[FD_DB_MAGIC_SIZE] = {
    'f', 'd', '_', 'm', 'g', 'k', '1', '\0',
};


char const *fd_db_strerror(int code)
{
    return FD_DB_ERROR_STR[code];
}

int fd_db_open(void **handle, int mode, char const *path)
{
    int retval      = FD_DBERR_SUCCESS;
    errno_t errcode = 0;

    switch (mode) {
    case FD_DBOM_READ: {
        errcode = fopen_s(*handle, path, "rb+");
    } break;
    case FD_DBOM_WRITE: {
        errcode = fopen_s(*handle, path, "wb+");
    } break;
    default: {
        retval = FD_DBERR_MINVALID;
        FD_LOG_ERROR("%s: %d", fd_db_strerror(retval), mode);
        return retval;
    } break;
    }

    if (errcode != 0) {
        retval = FD_DBERR_NOFILE;
        FD_LOG_ERROR("%s: \'%s\'", fd_db_strerror(retval), path);
        return retval;
    }

    return retval;
}

int fd_db_close(void **handle)
{
    int retval = FD_DBERR_SUCCESS;

    if (!(*handle)) {
        retval = FD_DBERR_HANDLE;
        FD_LOG_ERROR("%s: %p", fd_db_strerror(retval), (void *)(handle));

        return retval;
    }

    fclose(*handle);

    return retval;
}

int fd_db_read(void **handle, struct fd_db_all *data_ptr)
{
    int retval = FD_DBERR_SUCCESS;

    struct fd_db_header *header = NULL;
    struct fd_db_config *config = NULL;
    struct fd_db_dir *dirs      = NULL;
    struct fd_db_file *files    = NULL;
    struct fd_db_all *data      = NULL;

    size_t head_size   = sizeof *header;
    size_t config_size = sizeof *config;
    size_t dir_size    = sizeof *dirs;
    size_t file_size   = sizeof *files;

    size_t total     = head_size + config_size + dir_size + file_size;
    size_t data_size = sizeof(*data);

    // Alignment might cause this
    if (total > data_size)
        data_size = total;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"

    /* Allocate and read header */
    header = malloc(head_size);
    if (!header) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for database header", fd_db_strerror(retval));

        return FD_DBERR_ALLOC;
    }

    if ((retval = db$read_header(handle, header)) != FD_DBERR_SUCCESS)
        return retval;

    /* Allocate and read config */
    config = malloc(config_size);
    if (!config) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for database config", fd_db_strerror(retval));

        return FD_DBERR_ALLOC;
    }

    if ((retval = db$read_config(handle, config)) != FD_DBERR_SUCCESS)
        return retval;

    /* Allocate and read directory entries */
    dirs = calloc(header->dirs_count, dir_size);
    if (!dirs) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for database directory entries", fd_db_strerror(retval));

        return FD_DBERR_ALLOC;
    }

    for (size_t i = 0; i < header->dirs_count; ++i) {
        if ((retval = db$read_dir(handle, &dirs[i])) != FD_DBERR_SUCCESS)
            return retval;
    }

    /* Allocate and read file entries */
    files = calloc(header->files_count, file_size);
    if (!files) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for database file entries", fd_db_strerror(retval));

        return FD_DBERR_ALLOC;
    }

    for (size_t i = 0; i < header->files_count; ++i) {
        if ((retval = db$read_file(handle, &files[i])) != FD_DBERR_SUCCESS)
            return retval;
    }

#pragma clang diagnostic pop

    // Allocate and populate all database entries
    data = malloc(data_size);
    if (!data) {
        retval = FD_DBERR_ALLOC;
        FD_LOG_ERROR("%s for database data", fd_db_strerror(retval));

        return FD_DBERR_ALLOC;
    }

    FD_LOG_TRACE("copying header");
    data->header = malloc(sizeof *header);
    memcpy(data->header, header, sizeof *header);

    FD_LOG_TRACE("copying config");
    data->config = malloc(sizeof *config);
    memcpy(data->config, config, sizeof *config);

    FD_LOG_TRACE("copying directories");
    data->dirs = calloc(header->dirs_count, sizeof *dirs);
    memcpy(data->dirs, dirs, sizeof *dirs * header->dirs_count);

    FD_LOG_TRACE("copying files");
    data->files = calloc(header->files_count, sizeof *files);
    memcpy(data->files, files, sizeof *files * header->files_count);

    data_ptr = data;

    // Free all resources
    free(header);
    free(config);
    free(dirs);
    free(files);

    return retval;
}

int fd_db_write(void **handle, struct fd_db_all const *data)
{
    int retval = FD_DBERR_SUCCESS;

    struct fd_db_header *header  = NULL;
    struct fd_db_config *config  = NULL;
    struct fd_db_dir *dirs       = NULL;
    struct fd_db_dir *dirs_end   = NULL;
    struct fd_db_file *files     = NULL;
    struct fd_db_file *files_end = NULL;

    if (!data) {
        retval = FD_DBERR_NULLBUF;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    // Populate header
    header = data->header;
    if (!header) {
        retval = FD_DBERR_NULLBUF;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    // Populate config
    config = data->config;
    if (!config) {
        retval = FD_DBERR_NULLBUF;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }

    // Populate directory entries iterator pair
    dirs = data->dirs;
    if (!dirs) {
        retval = FD_DBERR_NULLBUF;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }
    dirs_end = dirs + header->dirs_count;

    // Populate file entries iterator pair
    files = data->files;
    if (!files) {
        retval = FD_DBERR_NULLBUF;
        FD_LOG_ERROR("%s", fd_db_strerror(retval));

        return retval;
    }
    files_end = files + header->files_count;

    // Write header
    if ((retval = db$write_header(handle, header)) != FD_DBERR_SUCCESS)
        return retval;

    // Write config
    if ((retval = db$write_config(handle, config)) != FD_DBERR_SUCCESS)
        return retval;

    // Write directory entries
    for (; dirs != dirs_end; ++dirs) {
        if ((retval = db$write_dir(handle, dirs)) != FD_DBERR_SUCCESS)
            return retval;
    }

    // Write file entries
    for (; files != files_end; ++files) {
        if ((retval = db$write_file(handle, files)) != FD_DBERR_SUCCESS)
            return retval;
    }


    return retval;
}
