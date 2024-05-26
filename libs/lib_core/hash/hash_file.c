/* ===========================================================================
    Information:
        lib/hash/hash_file.c   0.1.0 2024/05/12 <claymore>
    Description:
        file hashing, encoding and decoding
   ========================================================================= */

#include "hash_file.h"
#include "logging/log.h"

#include <stdio.h>
#include <stdlib.h>

#include <openssl/evp.h>

FD_INTERNAL int fd$open_file(char *path, char **buffer)
{
    int retval     = 0;
    size_t pathlen = 0;

    pathlen = strlen(path);
    *buffer = malloc(pathlen);

    return retval;
}

char *fd_hash_file(struct fd_file_info *fi)
{
    EVP_MD_CTX *mdctx = NULL;
    EVP_MD *md        = NULL;
    int err           = 0;
    char *filebuf     = NULL;

    char *buf = malloc(EVP_MAX_MD_SIZE);
    if (!buf) {
        FD_LOG_ERROR("failed to alloc digest buffer");
    }

    md = EVP_MD_fetch(NULL, "MD5", NULL);
    if (!md) {
        FD_LOG_ERROR("unknown message digest");
        exit(1);
    }
    mdctx = EVP_MD_CTX_new();

    err = EVP_DigestInit_ex2(mdctx, md, NULL);
    if (!err) {
        FD_LOG_ERROR("digest initialization failed");
        exit(1);
    }

    EVP_MD_CTX_free(mdctx);
    EVP_MD_free(md);

    return buf;
}
