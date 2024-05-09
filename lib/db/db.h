/* ===========================================================================
    Information:
        @(#)db/db.h   0.1.0 2024/05/04
        lib/db/db.h   0.1.0 <claymore>
    Description:
        database index for fetched files and paths
   ========================================================================= */
#ifndef FD_DB
#define FD_DB
#pragma once

#include "utils/defs.h"

#define FD_DB_MAGIC_SIZE  (8)
#define FD_DB_PATH_MAXLEN (512)

FD_EXTERN_BEGIN

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

enum FD_DB_OPENMODE
{
    FD_DBOM_READ,
    FD_DBOM_WRITE,
};

extern char const *FD_DB_ERROR_STR[];

struct fd_db_header
{
    uint8_t magic[FD_DB_MAGIC_SIZE];
    uint32_t version;
    size_t dirs_count;
    size_t files_count;
} FD_ALIGNED;

struct fd_db_config
{
    bool verbose;
    char scan_root[FD_DB_PATH_MAXLEN];
    char path[FD_DB_PATH_MAXLEN];
} FD_ALIGNED;

struct fd_db_dir
{
    uint64_t time_sec;
    uint32_t time_nsec;
    char path[FD_DB_PATH_MAXLEN];
} FD_ALIGNED;

struct fd_db_file
{
    int type;
    size_t size;
    char path[FD_DB_PATH_MAXLEN];
} FD_ALIGNED;

struct fd_db_all
{
    struct fd_db_header *header;
    struct fd_db_config *config;
    struct fd_db_dir *dirs;
    struct fd_db_file *files;
} FD_ALIGNED;

extern uint8_t const FD_DB_MAGIC[FD_DB_MAGIC_SIZE];

FD_EXPORT char const *fd_db_strerror(int code);

FD_EXPORT int fd_db_open(void **, int, char const *);

FD_EXPORT int fd_db_close(void **);

FD_EXPORT int fd_db_read(void **, struct fd_db_all *);

FD_EXPORT int fd_db_write(void **, struct fd_db_all const *);

FD_EXTERN_END

#endif /* !FD_DB */
