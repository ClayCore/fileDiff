/* ===========================================================================
    Information:
        @(#)logging/log.c   0.1.0 2024/04/28
        lib/logging/log.c   0.1.0 <claymore>
    Description:
        internal logging library
   ========================================================================= */

#include "log.h"

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

#define FD_LOG_MAXCB (64)
#define FD_LOG_BUFSZ (16)

typedef struct
{
    fdlib_log_func func;
    void *data;
    i32 level;
} callback;

global struct
{
    void *data;
    fdlib_log_lock lock;
    i32 level;
    bool quiet;
    callback callbacks[FD_LOG_MAXCB];
} FD_LOGGER;

internal char const *const FD_LOG_LEVELS[] = {
    [FD_LL_TRACE] = "TRACE",  //
    [FD_LL_DEBUG] = "DEBUG",  //
    [FD_LL_INFO]  = "INFO",   //
    [FD_LL_WARN]  = "WARN",   //
    [FD_LL_ERROR] = "ERROR",  //
};

internal char const *const FD_LOG_COLORS[] = {
    [FD_LL_TRACE] = "\x1b[94m",  //
    [FD_LL_DEBUG] = "\x1b[36m",  //
    [FD_LL_INFO]  = "\x1b[32m",  //
    [FD_LL_WARN]  = "\x1b[33m",  //
    [FD_LL_ERROR] = "\x1b[31m",  //
};

internal void logger_lock(void)
{
    if (FD_LOGGER.lock)
        FD_LOGGER.lock(true, FD_LOGGER.data);
}

internal void logger_unlock(void)
{
    if (FD_LOGGER.lock)
        FD_LOGGER.lock(false, FD_LOGGER.data);
}

internal void callback_stdout(fdlib_log_record *r)
{
    char buf[FD_LOG_BUFSZ]                               = { 0 };
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", r->time)] = '\0';

    fprintf(
        r->stream,                //
        "%s %s%-5s\x1b[0m "       //
        "\xb[90m%s:%d:\x1b[0m ",  //
        buf,                      //
        FD_LOG_COLORS[r->level],  //
        FD_LOG_LEVELS[r->level],  //
        r->file, r->line          //
    );

    vfprintf(r->stream, r->fmt, r->ap);
    fprintf(r->stream, "\n");
    fflush(r->stream);
}

internal void record_init(fdlib_log_record *r, void *s)
{
    if (!r->time) {
        time_t tm = time(NULL);
        r->time   = localtime(&tm);
    }

    r->stream = s;
}

char const *fdlib_loglevel_str(i32 l)
{
    return FD_LOG_LEVELS[l];
}

void fdlib_log_set_lock(fdlib_log_lock fn, void *d)
{
    FD_LOGGER.lock = fn;
    FD_LOGGER.data = d;
}

void fdlib_log_set_level(i32 l)
{
    FD_LOGGER.level = l;
}

void fdlib_log_set_quiet(bool is_quiet)
{
    FD_LOGGER.quiet = is_quiet;
}

i32 fdlib_log_add_callback(fdlib_log_func fn, void *d, i32 l)
{
    for (i32 i = 0; i < FD_LOG_MAXCB; ++i) {
        if (!FD_LOGGER.callbacks[i].func) {
            FD_LOGGER.callbacks[i] = (callback) {
                .func  = fn,  //
                .data  = d,   //
                .level = l    //
            };

            return 0;
        }
    }

    return -1;
}

i32 fdlib_log_add_file(FILE *fp, i32 l)
{
    return fdlib_log_add_callback(callback_stdout, fp, l);
}

void fdlib_log(i32 lv, char *fp, i32 ln, char *fmt, ...)
{
    fdlib_log_record r = {
        .fmt   = fmt,  //
        .file  = fp,   //
        .line  = ln,   //
        .level = lv,   //
    };

    logger_lock();

    if (!FD_LOGGER.quiet and lv >= FD_LOGGER.level) {
        record_init(&r, stderr);

        va_start(r.ap, fmt);
        callback_stdout(&r);
        va_end(r.ap);
    }

    for (i32 i = 0; i < FD_LOG_MAXCB and FD_LOGGER.callbacks[i].func; ++i) {
        callback *cb = &FD_LOGGER.callbacks[i];
        if (lv >= cb->level) {
            record_init(&r, cb->data);

            va_start(r.ap, fmt);
            callback_stdout(&r);
            va_end(r.ap);
        }
    }
    logger_unlock();
}
