/**
 *    @file  log.c
 *   @brief  Log wrapper interface
 */

#include <stdio.h>
#include <stdarg.h>

#include "common.h"
#include "utils.h"
#include "log.h"
#include "elfo.h"

i32 _log_it(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r = 0;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}


i32 _log_info(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r = 0;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[N] ");
#endif
    fprintf(stdout, "[%s:%d] ", f, l);
    r = vfprintf(stdout, fmt, ap);
    va_end(ap);
    return r;
}

i32 logf_it(FILE *f, const sbyte* fmt, ...)
{
    i32 r = 0;
    va_list ap;
    va_start(ap, fmt);
    r = vfprintf(f, fmt, ap);
    //XXX: decide if there will be newline
    va_end(ap);
    return r;
}

i32 logf_info(FILE *f, const sbyte* fmt, ...) // XXX: add __LINE__
{
    i32 r = 0;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(f, "[N] ");
#endif
    r = vfprintf(f, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

i32 logf_debug(FILE *f, const sbyte* fmt, ...)
{
    i32 r = 0;
#ifdef DEBUG
    va_list ap;
    va_start(ap, fmt);
    fprintf(f, "[D] ");
    r = vfprintf(f, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
#endif
    return r;
}

i32 _log_debug(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r = 0;
#ifdef DEBUG
    va_list ap;
    va_start(ap, fmt);
    fprintf(stdout, "[D] ");
    fprintf(stdout, "[%s:%d] ", f, l);
    r = vfprintf(stdout, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
#endif
    return r;
}

i32 _log_warning(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[W] ");
    fprintf(stdout, "[%s:%d] ", f, l);
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

i32 _log_error(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stderr, "[E] ");
    fprintf(stderr, "[%s:%d] ", f, l);
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
#ifdef ABORT_ON_ERROR
    abort();
#endif
    return r;
}

i32 _log_fatal(const char *f, int l, const sbyte* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[!] ");
    fprintf(stdout, "[%s:%d] ", f, l);
#endif
    vfprintf(stderr, fmt, ap);
    fprintf(stdout, " : aborting...\n");
    va_end(ap);
    abort();
}

i32 _log_info_buff(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[NB] ");
    fprintf(stdout, "[%s:%d] ", f, l);
    fprintf(stdout, "\n");
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

i32 _log_debug_buff(const char *f, int l, const sbyte* fmt, ...)
{
    i32 r;
    va_list ap;
    va_start(ap, fmt);
#ifdef DEBUG
    fprintf(stdout, "[DB] ");
    fprintf(stdout, "[%s:%d] ", f, l);
    fprintf(stdout, "\n");
#endif
    r = vfprintf(stderr, fmt, ap);
    fprintf(stdout, "\n");
    va_end(ap);
    return r;
}

