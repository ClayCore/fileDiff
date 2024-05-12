/* ===========================================================================
    Information:
        lib/platform/win32/path.c   0.1.0 2024/04/28 <claymore>
    Description:
        win32 path manipulation
   ========================================================================= */

#include "path.h"
#include "logging/log.h"
#include "utils/defs.h"
#include "win32.h"


#include <ctype.h>
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


FD_GLOBAL char const *fd$type_str[] = {
    [FD_PT_ABSOLUTE]       = "absolute",                   //
    [FD_PT_INVALID]        = "invalid",                    //
    [FD_PT_NETWORK]        = "network",                    //
    [FD_PT_RELATIVE_DRIVE] = "relative to current drive",  //
    [FD_PT_RELATIVE_ROOT]  = "relative to current root",   //
    [FD_PT_RELATIVE]       = "relative",                   //
    [FD_PT_UNC]            = "unc/dos-device"              //
};

FD_INTERNAL bool fd$is_abs(char const *, size_t);
FD_INTERNAL bool fd$is_network(char const *, size_t);
FD_INTERNAL bool fd$is_rel_drive(char const *, size_t);
FD_INTERNAL bool fd$is_rel_root(char const *, size_t);
FD_INTERNAL bool fd$is_rel(char const *, size_t);
FD_INTERNAL bool fd$is_unc(char const *, size_t);

FD_GLOBAL int _fd_type_iter[] = {
    [FD_PT_ABSOLUTE]       = FD_PT_ABSOLUTE,
    [FD_PT_NETWORK]        = FD_PT_NETWORK,
    [FD_PT_RELATIVE_DRIVE] = FD_PT_RELATIVE_DRIVE,
    [FD_PT_RELATIVE_ROOT]  = FD_PT_RELATIVE_ROOT,
    [FD_PT_RELATIVE]       = FD_PT_RELATIVE,
    [FD_PT_UNC]            = FD_PT_UNC,
    [FD_PT_INVALID]        = FD_PT_INVALID,
};

FD_GLOBAL bool (*fd$pt_jmp_table[])(char const *, size_t) = {
    [FD_PT_ABSOLUTE]       = fd$is_abs,
    [FD_PT_NETWORK]        = fd$is_network,
    [FD_PT_RELATIVE_DRIVE] = fd$is_rel_drive,
    [FD_PT_RELATIVE_ROOT]  = fd$is_rel_root,
    [FD_PT_RELATIVE]       = fd$is_rel,
    [FD_PT_UNC]            = fd$is_unc,
};
FD_GLOBAL size_t fd$pt_jmp_table_len = FD_ARRAYSZ(fd$pt_jmp_table);

FD_INTERNAL bool fd$check_all(size_t numchecks, bool const checks[static numchecks])
{
    bool result = true;
    for (size_t i = 0; i < numchecks; ++i) {
        if (!checks[i]) {
            result = false;
            break;
        }
    }

    return result;
}

FD_INTERNAL bool fd$is_abs(char const *path, size_t path_len)
{
    if (path_len <= 3)
        return false;

    bool checks[] = {
        isalpha(path[0]),  //
        path[1] == ':',    //
        path[2] == '\\',   //
    };
    size_t numchecks = FD_ARRAYSZ(checks);

    return fd$check_all(numchecks, checks);
}

FD_INTERNAL bool fd$is_network(char const *path, size_t path_len)
{
    if (path_len <= 3)
        return false;

    bool checks[] = {
        path[0] == '\\',   //
        path[1] == '\\',   //
        isalpha(path[2]),  //
    };
    size_t numchecks = FD_ARRAYSZ(checks);

    return fd$check_all(numchecks, checks);
}

FD_INTERNAL bool fd$is_rel_drive(char const *path, size_t path_len)
{
    if (path_len <= 3)
        return false;

    bool checks[] = {
        isalpha(path[0]),    //
        path[1] == ':',      //
        !(path[2] == '\\'),  //
    };
    size_t numchecks = FD_ARRAYSZ(checks);

    return fd$check_all(numchecks, checks);
}

FD_INTERNAL bool fd$is_rel_root(char const *path, size_t path_len)
{
    if (path_len <= 2)
        return false;

    bool checks[] = {
        path[0] == '\\',     //
        !(path[1] == '\\'),  //
    };
    size_t numchecks = FD_ARRAYSZ(checks);

    return fd$check_all(numchecks, checks);
}

FD_INTERNAL bool fd$is_rel(char const *path, size_t path_len)
{
    bool checks[] = {
        !(fd$pt_jmp_table[FD_PT_ABSOLUTE](path, path_len)),        //
        !(fd$pt_jmp_table[FD_PT_NETWORK](path, path_len)),         //
        !(fd$pt_jmp_table[FD_PT_RELATIVE_DRIVE](path, path_len)),  //
        !(fd$pt_jmp_table[FD_PT_RELATIVE_ROOT](path, path_len)),   //
        !(fd$pt_jmp_table[FD_PT_UNC](path, path_len)),             //
    };
    size_t numchecks = FD_ARRAYSZ(checks);

    return fd$check_all(numchecks, checks);
}

FD_INTERNAL bool fd$is_unc(char const *path, size_t path_len)
{
    if (path_len <= 3)
        return false;

    bool checks[] = {
        path[0] == '\\',                  //
        path[1] == '\\',                  //
        path[2] == '?' || path[2] == '.'  //
    };
    size_t numchecks = FD_ARRAYSZ(checks);

    return fd$check_all(numchecks, checks);
}

#pragma clang diagnostic pop

char *fdlib_win32_normalize_path(char *path)
{
    enum FD_WIN32_PATH_TYPE ft = FD_PT_INVALID;
    size_t path_len            = strlen(path);

    for (size_t i = 0; i < fd$pt_jmp_table_len; ++i) {
        if (fd$pt_jmp_table[i](path, path_len)) {
            ft = _fd_type_iter[i];
        }
    }

    if (ft == FD_PT_INVALID) {
        FD_LOG_ERROR("unrecognized path format, \'%s\'", path);
        exit(-1);
    }
    FD_LOG_INFO("[path type]:\t\'%s\'", fd$type_str[ft]);

    // TODO: return normalized path
    return path;
}
