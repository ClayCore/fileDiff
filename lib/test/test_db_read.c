#include "db/db.h"

#include <stdio.h>

int main(void)
{
    FILE *f     = NULL;
    int errcode = 0;

    struct fd_db_all db = { 0 };

    fprintf(stderr, "!!!!! test_db_read !!!!!\n");

    errcode = fd_db_open((void **)(&f), FD_DBOM_READ, "hidden$db.dat");
    if (errcode != FD_DBERR_SUCCESS) {
        fprintf(stderr, "open failed\n");
        return 1;
    }

    errcode = fd_db_read((void **)(&f), &db);
    if (errcode != FD_DBERR_SUCCESS) {
        fprintf(stderr, "read failed\n");
        return 2;
    }

    errcode = fd_db_close((void **)(&f));
    if (errcode != FD_DBERR_SUCCESS) {
        fprintf(stderr, "close failed\n");
        return 3;
    }

    return 0;
}
