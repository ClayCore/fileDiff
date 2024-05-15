/* ===========================================================================
    Information:
        lib/hash/hash_file.h   0.1.0 2024/05/12 <claymore>
    Description:
        file hashing, encoding and decoding
   ========================================================================= */

#ifndef FD_HASH_HASH_FILE
#define FD_HASH_HASH_FILE
#pragma once

#include "platform/os.h"
#include "utils/defs.h"

FD_EXTERN_BEGIN

FD_EXPORT char *fd_hash_file(struct fd_file_info *fi);

FD_EXTERN_END

#endif /* !FD_HASH_HASH_FILE */
