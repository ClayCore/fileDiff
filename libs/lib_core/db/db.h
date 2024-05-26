/* ===========================================================================
    Information:
        lib/db/db.h   0.1.0 2024/05/04 <claymore>
    Description:
        database index for fetched files and paths
   ========================================================================= */
#ifndef FD_DB
#define FD_DB
#pragma once

#include "utils/defs.h"

/* Length of magic string embedded in the header of the database */
#define FD_DB_MAGIC_SIZE (8)

/* Maximum allowed path length in any of the strings in databse structures */
#define FD_DB_PATH_MAXLEN (512)

FD_EXTERN_BEGIN

/* ---------------------------------------------------------------------------
    Name:
        FD_DB_ERRORS
    Description:
        describes error codes returned from database functions
    Fields:
        | name              | description                          |
        | ================= | ==================================== |
        | FD_DBERR_SUCCESS  | no error occured                     |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_NOFILE   | no database found at specified path  |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_ALLOC    | memory allocation failed             |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_READ     | read operation failed                |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_WRITE    | write operation failed               |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_MINVALID | file open mode invalid               |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_HANDLE   | handle to database file is invalid   |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_HANDLE   | invalid handle to database file      |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_MAGIC    | magic string in header doesn't match |
        | ----------------- | ------------------------------------ |
        | FD_DBERR_NULLBUF  | specified buffer is null             |
   ------------------------------------------------------------------------- */
enum FD_DB_ERRORS
{
    FD_DBERR_SUCCESS,
    FD_DBERR_NOFILE,
    FD_DBERR_ALLOC,
    FD_DBERR_READ,
    FD_DBERR_WRITE,
    FD_DBERR_MINVALID,
    FD_DBERR_HANDLE,
    FD_DBERR_MAGIC,
    FD_DBERR_NULLBUF,
};

/* Maps above error values to strings for display */
extern char const *FD_DB_ERROR_STR[];

/* Magic header string */
extern uint8_t const FD_DB_MAGIC[FD_DB_MAGIC_SIZE];

/* ---------------------------------------------------------------------------
    Name:
        FD_DB_OPENMODE
    Description:
        describes modes of opening the main database file
    Fields:
        | name          | description           |
        | ============= | ===================== |
        | FD_DBOM_READ  | read/create file      |
        | ------------- | --------------------- |
        | FD_DBOM_WRITE | write/update database |
   ------------------------------------------------------------------------- */
enum FD_DB_OPENMODE
{
    FD_DBOM_READ,
    FD_DBOM_WRITE,
};

/* ---------------------------------------------------------------------------
    Name:
        fd_db_header
    Description:
        contains database header structure
    Fields:
        | type      | name        | description                         |
        | --------- | ----------- | ----------------------------------- |
        | uint8_t[] | magic       | magic header, see FD_DB_MAGIC       |
        | uint32_t  | version     | database version                    |
        | size_t    | dirs_count  | number of directory entries present |
        | size_t    | files_count | number of file entries present      |
   ------------------------------------------------------------------------- */
struct fd_db_header
{
    uint8_t magic[FD_DB_MAGIC_SIZE];
    uint32_t version;
    size_t dirs_count;
    size_t files_count;
} FD_ALIGNED;

/* ---------------------------------------------------------------------------
    Name:
        fd_db_config
    Description:
        describes database configuration
    Fields:
        | type   | name      | description                            |
        | ------ | --------- | -------------------------------------- |
        | char[] | scan_root | where to begin scanning for dirs/files |
        | char[] | path      | absolute path to database file         |
   ------------------------------------------------------------------------- */
struct fd_db_config
{
    char scan_root[FD_DB_PATH_MAXLEN];
    char path[FD_DB_PATH_MAXLEN];
} FD_ALIGNED;

/* ---------------------------------------------------------------------------
    Name:
        fd_db_dir
    Description:
        describes a directory entry in the filesystem
    Fields:
        | type     | name  | description                           |
        | -------- | ----- | ------------------------------------- |
        | int64_t  | atime | last access timestamp                 |
        | char[]   | path  | absolute path to a specific directory |
   ------------------------------------------------------------------------- */
struct fd_db_dir
{
    int64_t atime;
    char path[FD_DB_PATH_MAXLEN];
} FD_ALIGNED;

/* ---------------------------------------------------------------------------
    Name:
        fd_db_file
    Description:
        describes a file entry in the filesystem
    Fields:
        | type     | name  | description                      |
        | -------- | ----- | -------------------------------- |
        | int64_t  | atime | last access timestamp            |
        | size_t   | size  | size of file in bytes            |
        | int      | type  | type of file(symlink, etc.)      |
        | char[]   | path  | absolute path to a specific file |
   ------------------------------------------------------------------------- */
struct fd_db_file
{
    int64_t atime;
    size_t size;
    int type;
    char path[FD_DB_PATH_MAXLEN];
} FD_ALIGNED;

/* ---------------------------------------------------------------------------
    Name:
        fd_db_all
    Description:
        convenience type for holding all database information
    Fields:
        | type           | name   | description                     |
        | -------------- | ------ | ------------------------------- |
        | fd_db_header * | header | pointer to header               |
        | fd_db_config * | config | pointer to configuration        |
        | fd_db_dir *    | dirs   | pointer to directory entry list |
        | fd_db_file *   | files  | pointer to file entry list      |
   ------------------------------------------------------------------------- */
struct fd_db_all
{
    struct fd_db_header *header;
    struct fd_db_config *config;
    struct fd_db_dir *dirs;
    struct fd_db_file *files;
} FD_ALIGNED;

/* ---------------------------------------------------------------------------
    Name:
        fd_db_strerror
    Purpose:
        returns display string representing the returned error
    Arguments:
        | type | name | description      |
        | ---- | ---- | ---------------- |
        | int  | code | valid error code |
    Return value:
        | type         | description       |
        | ------------ | ----------------- |
        | char const * | stringified error |
   ------------------------------------------------------------------------- */
FD_EXPORT char const *fd_db_strerror(int code);

/* ---------------------------------------------------------------------------
    Name:
        fd_db_open
    Purpose:
        open handle to database file
    Arguments:
        | type         | name   | description                    |
        | ------------ | ------ | ------------------------------ |
        | void **      | handle | owned pointer to database file |
        | int          | mode   | FD_DBOM_READ or FD_DBOM_WRITE  |
        | char const * | path   | absolute path to database file |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
FD_EXPORT int fd_db_open(void **handle, int mode, char const *path);

/* ---------------------------------------------------------------------------
    Name:
        fd_db_close
    Purpose:
        close handle to database file
    Arguments:
        | type    | name   | description                    |
        | ------- | ------ | ------------------------------ |
        | void ** | handle | owned pointer to database file |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
FD_EXPORT int fd_db_close(void **handle);

/* ---------------------------------------------------------------------------
    Name:
        fd_db_read
    Purpose:
        read all data from database handle
    Arguments:
        | type        | name   | description                    |
        | ----------- | ------ | ------------------------------ |
        | void **     | handle | owned pointer to database file |
        | fd_db_all * | data   | out param, saves all data read |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
FD_EXPORT int fd_db_read(void **handle, struct fd_db_all *data);

/* ---------------------------------------------------------------------------
    Name:
        fd_db_write
    Purpose:
        write all data to database handle
    Arguments:
        | type              | name   | description                    |
        | ----------------- | ------ | ------------------------------ |
        | void **           | handle | owned pointer to database file |
        | fd_db_all const * | data   | in param, data to write        |
    Return value:
        | type | description                |
        | ---- | -------------------------- |
        | int  | one of FD_DB_ERRORS values |
   ------------------------------------------------------------------------- */
FD_EXPORT int fd_db_write(void **handle, struct fd_db_all const *data);

FD_EXTERN_END

#endif /* !FD_DB */
