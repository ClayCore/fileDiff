#include "db/db.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f     = NULL;
    int errcode = 0;

    struct fd_db_config *cfg  = calloc(1, sizeof *cfg);
    struct fd_db_header *head = calloc(1, sizeof *head);
    struct fd_db_dir *dirs    = calloc(1, sizeof *dirs);
    struct fd_db_file *files  = calloc(1, sizeof *files);
    struct fd_db_all *db      = calloc(1, sizeof *db);

    uint8_t const magic[FD_DB_MAGIC_SIZE] = {
        'f', 'd', '_', 'm', 'g', 'k', '1', '\0',
    };

    fprintf(stderr, "!!!!! test_db_write !!!!!\n");

    head->version     = 1;
    head->dirs_count  = 1;
    head->files_count = 1;
    for (int i = 0; i < FD_DB_MAGIC_SIZE; ++i) {
        head->magic[i] = magic[i];
    }

    strcpy(cfg->path, "hidden_db.dat");
    strcpy(cfg->scan_root, "hidden_dir/");

    dirs->atime = 10;
    strcpy(dirs->path, "hidden_dir/");

    files->atime = 10;
    files->size  = 1;
    files->type  = 1;
    strcpy(dirs->path, "hidden_db.dat");

    db->header = head;
    db->config = cfg;
    db->dirs   = dirs;
    db->files  = files;

    errcode = fd_db_open((void **)(&f), FD_DBOM_WRITE, "hidden$db.dat");
    if (errcode != FD_DBERR_SUCCESS) {
        fprintf(stderr, "open failed\n");
        return 1;
    }

    errcode = fd_db_write((void **)(&f), db);
    if (errcode != FD_DBERR_SUCCESS) {
        fprintf(stderr, "write failed\n");
        return 2;
    }

    errcode = fd_db_close((void **)(&f));
    if (errcode != FD_DBERR_SUCCESS) {
        fprintf(stderr, "close failed\n");
        return 3;
    }

    return 0;
}
