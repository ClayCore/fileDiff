/* ===========================================================================
    Information:
        @(#)logging/log.c   0.1.0 2024/04/28
        lib/logging/log.c   0.1.0 <claymore>
    Description:
        internal logging library
   ========================================================================= */

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FD_LOG_MAXCB (64)
#define FD_LOG_BUFSZ (64)


typedef struct
{
    fdlib_log_func func;
    void *data;
    int32_t level;
} callback;

FD_GLOBAL struct
{
    void *data;
    fdlib_log_lock lock;
    int32_t level;
    bool quiet;
    callback callbacks[FD_LOG_MAXCB];
} FD_LOGGER;

FD_INTERNAL char const *const FD_LOG_LEVELS[] = {
    [FD_LL_TRACE] = "TRACE",  //
    [FD_LL_DEBUG] = "DEBUG",  //
    [FD_LL_INFO]  = "INFO",   //
    [FD_LL_WARN]  = "WARN",   //
    [FD_LL_ERROR] = "ERROR",  //
};

FD_INTERNAL char const *const FD_LOG_COLORS[] = {
    [FD_LL_TRACE] = "\x1b[94m",  //
    [FD_LL_DEBUG] = "\x1b[36m",  //
    [FD_LL_INFO]  = "\x1b[32m",  //
    [FD_LL_WARN]  = "\x1b[33m",  //
    [FD_LL_ERROR] = "\x1b[31m",  //
};

FD_INTERNAL void logger_lock(void)
{
    if (FD_LOGGER.lock)
        FD_LOGGER.lock(true, FD_LOGGER.data);
}

FD_INTERNAL void logger_unlock(void)
{
    if (FD_LOGGER.lock)
        FD_LOGGER.lock(false, FD_LOGGER.data);
}

FD_INTERNAL void callback_stdout(fdlib_log_record *r)
{
    char buf[FD_LOG_BUFSZ]                               = { 0 };
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", r->time)] = '\0';

    fprintf(
        r->stream,                    //
        "%s %s%-5s\x1b[0m "           //
        "\x1b[90m%s:%d:\x1b[0m\n\t",  //
        buf,                          //
        FD_LOG_COLORS[r->level],      //
        FD_LOG_LEVELS[r->level],      //
        r->file, r->line              //
    );

    vfprintf_s(r->stream, r->fmt, r->ap);
    fprintf(r->stream, "\n");
    fflush(r->stream);
}

FD_INTERNAL void record_init(fdlib_log_record *r, void *s)
{
    if (!r->time) {
        time_t t = time(NULL);

        r->time = localtime(&t);
    }

    r->stream = s;
}

FD_NO_EXPORT char const *fdlib_loglevel_str(int32_t l)
{
    return FD_LOG_LEVELS[l];
}

FD_NO_EXPORT void fdlib_log_set_lock(fdlib_log_lock fn, void *d)
{
    FD_LOGGER.lock = fn;
    FD_LOGGER.data = d;
}

FD_NO_EXPORT void fdlib_log_set_level(int32_t l)
{
    FD_LOGGER.level = l;
}

FD_NO_EXPORT void fdlib_log_set_quiet(bool is_quiet)
{
    FD_LOGGER.quiet = is_quiet;
}

FD_NO_EXPORT int32_t fdlib_log_add_callback(fdlib_log_func fn, void *d, int32_t l)
{
    for (int32_t i = 0; i < FD_LOG_MAXCB; ++i) {
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

FD_NO_EXPORT int32_t fdlib_log_add_file(FILE *fp, int32_t l)
{
    return fdlib_log_add_callback(callback_stdout, fp, l);
}

FD_NO_EXPORT void fdlib_log(int32_t lv, char *fp, int32_t ln, char *fmt, ...)
{
    fdlib_log_record r = {
        .fmt   = fmt,  //
        .file  = fp,   //
        .line  = ln,   //
        .level = lv,   //
    };

    logger_lock();

    if (!FD_LOGGER.quiet && lv >= FD_LOGGER.level) {
        record_init(&r, stderr);

        va_start(r.ap, fmt);
        callback_stdout(&r);
        va_end(r.ap);
    }

    for (int32_t i = 0; i < FD_LOG_MAXCB && FD_LOGGER.callbacks[i].func; ++i) {
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
