/* ===========================================================================
    Information:
        @(#)logging/log.h   0.1.0 2024/04/28
        lib/logging/log.h   0.1.0 <claymore>
    Description:
        internal logging library
   ========================================================================= */

#ifndef FD_LOGGING_LOG
#define FD_LOGGING_LOG
#pragma once

#include "types/aliases.h"
#include "utils/defs.h"

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif /* _CRT_SECURE_NO_WARNINGS */

#define FD_LOG_TRACE(...) (fdlib_log(FD_LL_TRACE, __FILE__, __LINE__, __VA_ARGS__))
#define FD_LOG_DEBUG(...) (fdlib_log(FD_LL_DEBUG, __FILE__, __LINE__, __VA_ARGS__))
#define FD_LOG_INFO(...)  (fdlib_log(FD_LL_INFO, __FILE__, __LINE__, __VA_ARGS__))
#define FD_LOG_WARN(...)  (fdlib_log(FD_LL_WARN, __FILE__, __LINE__, __VA_ARGS__))
#define FD_LOG_ERROR(...) (fdlib_log(FD_LL_ERROR, __FILE__, __LINE__, __VA_ARGS__))

FD_EXTERN_BEGIN

FD_NO_EXPORT enum fdlib_loglevel {
    FD_LL_TRACE,
    FD_LL_DEBUG,
    FD_LL_INFO,
    FD_LL_WARN,
    FD_LL_ERROR,
};

FD_NO_EXPORT typedef struct
{
    va_list ap;
    char *fmt;
    char *file;

    struct tm *time;

    void *stream;
    i32 line;
    i32 level;
} fdlib_log_record;

typedef void (*fdlib_log_func)(fdlib_log_record *);
typedef void (*fdlib_log_lock)(bool, void *);

FD_NO_EXPORT char const *fdlib_loglevel_str(i32);

FD_NO_EXPORT void fdlib_log_set_lock(fdlib_log_lock, void *);

FD_NO_EXPORT void fdlib_log_set_level(i32);

FD_NO_EXPORT void fdlib_log_set_quiet(bool);

FD_NO_EXPORT i32 fdlib_log_add_callback(fdlib_log_func, void *, i32);

FD_NO_EXPORT i32 fdlib_log_add_file(FILE *, i32);

FD_NO_EXPORT void fdlib_log(i32, char *, i32, char *, ...);

FD_EXTERN_END

#endif /* FD_LOGGING_LOG */
