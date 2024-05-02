/* ===========================================================================
    Information:
        @(#)platform/win32.c   0.1.0 2024/04/28
        lib/platform/win32.c   0.1.0 <claymore>
    Description:
        thin wrapper around win32 specific io operations
   ========================================================================= */

#ifdef DEV_WIN32

    #include "win32.h"
    #include "logging/log.h"

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif /* WIN32_LEAN_AND_MEAN */

    #ifndef _UNICODE
        #define _UNICODE
    #endif /* _UNICODE */
    #ifndef UNICODE
        #define UNICODE
    #endif /* UNICODE */

    #include <windows.h>

    #include <stdlib.h>
    #include <string.h>

enum FD_WIN32_PATH_TYPE
{
    FD_PT_ABSOLUTE,
    FD_PT_INVALID,
    FD_PT_NETWORK,
    FD_PT_RELATIVE_DRIVE,
    FD_PT_RELATIVE_ROOT,
    FD_PT_RELATIVE,
    FD_PT_UNC,
};

FD_GLOBAL char const *FD_WIN32_PATH_TYPE_STR[] = {
    [FD_PT_ABSOLUTE]       = "absolute",                   //
    [FD_PT_INVALID]        = "invalid",                    //
    [FD_PT_NETWORK]        = "network",                    //
    [FD_PT_RELATIVE_DRIVE] = "relative to current drive",  //
    [FD_PT_RELATIVE_ROOT]  = "relative to current root",   //
    [FD_PT_RELATIVE]       = "relative",                   //
    [FD_PT_UNC]            = "unc/dos-device"              //
};

// Function definitions for later use
FD_INTERNAL bool fdlib_path_is_absolute(char const *);
FD_INTERNAL bool fdlib_path_is_network(char const *);
FD_INTERNAL bool fdlib_path_is_relative_drive(char const *);
FD_INTERNAL bool fdlib_path_is_relative_root(char const *);
FD_INTERNAL bool fdlib_path_is_relative(char const *);
FD_INTERNAL bool fdlib_path_is_unc(char const *);

FD_INTERNAL bool validate_checks(int numchecks, bool checks[static numchecks])
{
    bool result = true;
    for (int i = 0; i < numchecks; ++i) {
        if (!checks[i]) {
            result = false;
            break;
        }
    }

    return result;
}

FD_INTERNAL bool fdlib_path_is_absolute(char const *path)
{
    bool checks[] = { isalpha(path[0]), path[1] == ':', path[2] == '\\' };
    int numchecks = sizeof(checks) / sizeof(*checks);

    return validate_checks(numchecks, checks);
}

FD_INTERNAL bool fdlib_path_is_network(char const *path)
{
    bool checks[] = { path[0] == '\\', path[1] == '\\', isalpha(path[2]) };
    int numchecks = sizeof(checks) / sizeof(*checks);

    return validate_checks(numchecks, checks);
}

FD_INTERNAL bool fdlib_path_is_relative_drive(char const *path)
{
    bool checks[] = { isalpha(path[0]), path[1] == ':', !(path[2] == '\\') };
    int numchecks = sizeof(checks) / sizeof(*checks);

    return validate_checks(numchecks, checks);
}

FD_INTERNAL bool fdlib_path_is_relative_root(char const *path)
{
    bool checks[] = { path[0] == '\\', !(path[1] == '\\') };
    int numchecks = sizeof(checks) / sizeof(*checks);

    return validate_checks(numchecks, checks);
}

FD_INTERNAL bool fdlib_path_is_relative(char const *path)
{
    bool checks[] = {
        !fdlib_path_is_absolute(path),
        !fdlib_path_is_network(path),
        !fdlib_path_is_relative_drive(path),
        !fdlib_path_is_relative_root(path),
        !fdlib_path_is_unc(path),
    };
    int numchecks = sizeof(checks) / sizeof(*checks);

    return validate_checks(numchecks, checks);
}

FD_INTERNAL bool fdlib_path_is_unc(char const *path)
{
    bool checks[] = { path[0] == '\\', path[1] == '\\', path[2] == '?' || path[2] == '.' };
    int numchecks = sizeof(checks) / sizeof(*checks);

    return validate_checks(numchecks, checks);
}


char *fdlib_win32_normalize_path(char *path)
{
    int path_type      = FD_PT_INVALID;
    uint_least64_t len = strlen(path);

    if (fdlib_path_is_relative(path)) {
        path_type = FD_PT_RELATIVE;
    } else if (fdlib_path_is_relative_root(path)) {
        path_type = FD_PT_RELATIVE_ROOT;
    } else if (fdlib_path_is_relative_drive(path)) {
        path_type = FD_PT_RELATIVE_DRIVE;
    } else if (fdlib_path_is_absolute(path)) {
        path_type = FD_PT_ABSOLUTE;
    } else if (fdlib_path_is_network(path)) {
        path_type = FD_PT_NETWORK;
    } else if (fdlib_path_is_unc(path)) {
        path_type = FD_PT_UNC;
    } else {
        FD_LOG_ERROR("unrecognized path format");
        exit(-1);
    }

    FD_LOG_INFO("[path_type]:\t\'%s\'", FD_WIN32_PATH_TYPE_STR[path_type]);


    // switch (path_type) {
    // case FD_PT_RELATIVE: {

    // } break;
    // case FD_PT_RELATIVE_DRIVE: {

    // } break;
    // case FD_PT_ABSOLUTE: {

    // } break;
    // case FD_PT_NETWORK: {

    // } break;
    // case FD_PT_UNC: {

    // } break;
    // default: {
    //     FD_LOG_ERROR("unrecognized path");
    // } break;
    // }
}

struct fdlib_file_info fdlib_win32_stat_file(char const *path)
{
    struct fdlib_file_info fi = { 0 };

    char out[MAX_PATH] = { 0 };
    char *out_ptr      = out;
    char **part        = NULL;

    GetFullPathNameA(path, MAX_PATH, out_ptr, part);

    FD_LOG_INFO("in_path: \'%s\'", path);
    FD_LOG_INFO("out_path: \'%s\'", out_ptr);

    size_t out_path_len = strlen(out);
    fi.namelen          = out_path_len;

    if (part != NULL) {
        FD_LOG_INFO("part_path: \'%s\'", *part);
    }

    fi.name = malloc(out_path_len + 1);
    if (!fi.name) {
        FD_LOG_ERROR("failed to alloc \'name\'");
        abort();
    }

    memcpy(fi.name, out_ptr, out_path_len);
    fi.name[out_path_len] = '\0';

    return fi;
}

#endif /* DEV_WIN32 */
