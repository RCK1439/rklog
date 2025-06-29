/**
 * rklog is a header-only logging library used for clear message logging/debugging
 * 
 * version: 1.1
 * author: Ruan C. Keet
 */

#ifndef RK_LOG_H
#define RK_LOG_H

#include <stdarg.h>

// --- rklog interface --------------------------------------------------------

/**
 * Logs a formatted message with an info tag to `stdout`
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogInfo(const char *fmt, ...);

/**
 * Logs a formatted message with a warning tag to `stdout`
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogWarning(const char *fmt, ...);

/**
 * Logs a formatted message with an error tag to `stdout`
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogError(const char *fmt, ...);

/**
 * Logs a formatted message with a fatal error tag to `stdout`
 *
 * @param[in] fmt
 *      The format specifier of the message
 */
void rkLogFatal(const char *fmt, ...);

/**
 * Logs a formatted message with an info tag to `stdout` with explicit
 * arguments
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogInfoArgs(const char *fmt, va_list args);

/**
 * Logs a formatted message with a warning tag to `stdout` with explicit
 * arguments
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogWarningArgs(const char *fmt, va_list args);

/**
 * Logs a formatted message with an error tag to `stdout` with explicit
 * arguments
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogErrorArgs(const char *fmt, va_list args);

/**
 * Logs a formatted message with a fatal error tag to `stdout` with explicit
 * arguments
 *
 * @param[in] fmt
 *      The format specifier of the message
 * @param[in] args
 *      The variadic arguments
 */
void rkLogFatalArgs(const char *fmt, va_list args);

#if defined(RK_LOG_IMPLEMENTATION)

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

// --- string tokens ----------------------------------------------------------

#define RK_TOKEN_ESCAPE_CODE_START "\033["
#define RK_TOKEN_ESCAPE_CODE_END   "m"

#define RK_TOKEN_ESCAPE_CODE_RESET RK_TOKEN_ESCAPE_CODE_START "0" RK_TOKEN_ESCAPE_CODE_END

#define RK_TOKEN_FOREGROUND "38;"
#define RK_TOKEN_BACKGROUND "48;"

#define RK_TOKEN_TRUE_COLOR_MODE "2;"
#define RK_TOKEN_256_COLOR_PALETTE_MODE "5;"

// --- formats ----------------------------------------------------------------

//                                                  foreground
//                                                  | true color mode
//                                                  | |  red channel
//                                                  | |  |  green channel
//                                                  | |  |  |  blue channel
//                                                  | |  |  |  |
#define RK_FMT_PRELUDE RK_TOKEN_ESCAPE_CODE_START "48;2;%d;%d;%d;38;2;%d;%d;%d" RK_TOKEN_ESCAPE_CODE_END
//                                                                | |  |  |  |
//                                                                | |  |  |  blue channel
//                                                                | |  |  green channel
//                                                                | |  red channel
//                                                                | true color mode
//                                                                background

#define RK_FMT_LABEL "[%s]:[%02d:%02d:%02d]: "
//                      |      |    |    |
//                      |      |    |    seconds
//                      |      |    minutes
//                      |      hours
//                      log level

#define RK_FMT_STDOUT "%s%s%s" RK_TOKEN_ESCAPE_CODE_RESET "\n"
//                      | | |
//                      | | log message
//                      | log label
//                      log prelude

// --- color literals ---------------------------------------------------------

#define RK_COLOR_GREEN  (rkColor) { 0, 255, 0 }
#define RK_COLOR_RED    (rkColor) { 255, 0, 0 }
#define RK_COLOR_YELLOW (rkColor) { 255, 255, 0 }
#define RK_COLOR_WHITE  (rkColor) { 255, 255, 255 }
#define RK_COLOR_BLACK  (rkColor) { 0, 0, 0 }

// --- constants --------------------------------------------------------------

#define MAX_PRELUDE_SIZE  64
#define MAX_LABEL_SIZE    64
#define MAX_MESSAGE_SIZE  256

// --- type definitions -------------------------------------------------------

/**
 * Descriptor enum describing the severity of the log message
 */
typedef enum rkLogLevel
{
    LOG_LEVEL_INFO        = 0,
    LOG_LEVEL_WARNING     = 1,
    LOG_LEVEL_ERROR       = 2,
    LOG_LEVEL_FATAL_ERROR = 3,
} rkLogLevel;

/**
 * Struct holding the system time at the point the log occured
 */
typedef struct rkTimeStamp
{
    uint32_t hours;   //--+
    uint32_t minutes; //  +-- pretty self explanatory
    uint32_t seconds; //--+
} rkTimeStamp;

/**
 * Struct describing 24-bit color for use with the ANSI codes
 */
typedef struct rkColor
{
    uint8_t r; // red channel
    uint8_t g; // green channel
    uint8_t b; // blue channel
} rkColor;

/**
 * Struct holding color information relating to log messages
 */
typedef struct rkLogStyle
{
    rkColor background; // background color
    rkColor foreground; // foreground color
} rkLogStyle;

/**
 * Struct containing the settings of the different log messages
 */
typedef struct rkConfig
{
    rkLogStyle infoStyle;       // style for info messages
    rkLogStyle warningStyle;    // style for warning messages
    rkLogStyle errorStyle;      // style for error messages
    rkLogStyle fatalErrorStyle; // style for fatal error messages
} rkConfig;

// --- global state -----------------------------------------------------------

// Tags used for the labels of the log messages 
static const char *s_Tags[] = {
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};

// Global state of the library
static const rkConfig s_State = {
    .infoStyle = {
        .background = RK_COLOR_BLACK,
        .foreground = RK_COLOR_GREEN
    },
    .warningStyle = {
        .background = RK_COLOR_BLACK,
        .foreground = RK_COLOR_YELLOW
    },
    .errorStyle = {
        .background = RK_COLOR_BLACK,
        .foreground = RK_COLOR_RED
    },
    .fatalErrorStyle = {
        .background = RK_COLOR_RED,
        .foreground = RK_COLOR_WHITE
    },
};

// --- utility functions ------------------------------------------------------

/**
 * Logs the formatted message with its configuration to `stdout`
 *
 * @param[in] level
 *      The severity level of the log message
 * @param[in] style
 *      The style in which the colors of the log message must look
 * @param[in] fmt
 *      The format of the message
 * @param[in] args
 *      Variadic arguments
 */
static void rkLogInternal(rkLogLevel level, rkLogStyle style, const char *fmt, va_list args);

/**
 * Formats `buf` to be the log message prelude
 *
 * @param[out] buf
 *      The buffer to format according to the prelude
 * @param[in] len
 *      The maximum length of the buffer
 * @param[in] background
 *      The background color of the text
 * @param[in] foreground
 *      The foreground color of the text
 */
static void rkGenPrelude(char *buf, size_t len, rkColor background, rkColor foreground);

/**
 * Formats `buf` to contain the log message tag and timestamp
 *
 * @param[out] buf
 *      The buffer to format according to the label
 * @param[in] len
 *      The maximum length of the buffer
 * @param[in] level
 *      The severity level of the log message
 */
static void rkGenLabel(char *buf, size_t len, rkLogLevel level);

/**
 * Queries the system's current local time 
 */
static rkTimeStamp rkGetCurrentTime(void);

// --- utility function implementation ----------------------------------------

static void rkLogInternal(rkLogLevel level, rkLogStyle style, const char *fmt, va_list args)
{
    char prelude[MAX_PRELUDE_SIZE+1] = { 0 };
    char label[MAX_LABEL_SIZE+1] = { 0 };
    char message[MAX_MESSAGE_SIZE+1] = { 0 };

    rkGenPrelude(prelude, MAX_PRELUDE_SIZE, style.foreground, style.background);
    rkGenLabel(label, MAX_LABEL_SIZE, level);

    vsnprintf(message, MAX_MESSAGE_SIZE, fmt, args);
    fprintf(stdout, RK_FMT_STDOUT, prelude, label, message);
}

static void rkGenPrelude(char *buf, size_t len, rkColor background, rkColor foreground)
{
    snprintf(buf, len, RK_FMT_PRELUDE,
        foreground.r, foreground.g, foreground.b,
        background.r, background.g, background.b
    );
}

static void rkGenLabel(char *buf, size_t len, rkLogLevel level)
{
    const char *label = s_Tags[level];
    const rkTimeStamp timeStamp = rkGetCurrentTime();
    
    snprintf(buf, len, RK_FMT_LABEL, label, timeStamp.hours, timeStamp.minutes, timeStamp.seconds);
}

static rkTimeStamp rkGetCurrentTime(void)
{
    struct tm *timeInfo;

    const time_t now = time(NULL);
    timeInfo = localtime(&now);
    
    const rkTimeStamp currTime = {
        .hours = (uint32_t)timeInfo->tm_hour,
        .minutes = (uint32_t)timeInfo->tm_min,
        .seconds = (uint32_t)timeInfo->tm_sec,
    };

    return currTime;
}

// --- rklog implementation ---------------------------------------------------

void rkLogInfo(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogInfoArgs(fmt, args);
    va_end(args);
}

void rkLogWarning(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogWarningArgs(fmt, args);
    va_end(args);
}

void rkLogError(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogErrorArgs(fmt, args);
    va_end(args);
}

void rkLogFatal(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    rkLogFatalArgs(fmt, args);
    va_end(args);
}

void rkLogInfoArgs(const char *fmt, va_list args)
{
    rkLogInternal(LOG_LEVEL_INFO, s_State.infoStyle, fmt, args);
}

void rkLogWarningArgs(const char *fmt, va_list args)
{
    rkLogInternal(LOG_LEVEL_WARNING, s_State.warningStyle, fmt, args);
}

void rkLogErrorArgs(const char *fmt, va_list args)
{
    rkLogInternal(LOG_LEVEL_ERROR, s_State.errorStyle, fmt, args);
}

void rkLogFatalArgs(const char *fmt, va_list args)
{
    rkLogInternal(LOG_LEVEL_FATAL_ERROR, s_State.fatalErrorStyle, fmt, args);
}

#endif /* RK_LOG_IMPLEMENTATION */

#endif /* RK_LOG_H */

