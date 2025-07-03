#pragma once

namespace rklog {

void Info(const char *fmt, ...);
void Warning(const char *fmt, ...);
void Error(const char *fmt, ...);
void Fatal(const char *fmt, ...);

#if defined(RK_LOG_IMPLEMENTATION)

#include "rklog.h"

void Info(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogInfoArgs(fmt, args);
    va_end(args);
}

void Warning(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogWarningArgs(fmt, args);
    va_end(args);
}

void Error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogErrorArgs(fmt, args);
    va_end(args);
}

void Fatal(const char *fmt, ...)
{
    va_list args;
    
    va_start(args, fmt);
    rkLogFatalArgs(fmt, args);
    va_end(args);
}

#endif

}
