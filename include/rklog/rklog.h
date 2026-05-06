#ifndef __RKLOG_H__
#define __RKLOG_H__

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define C_LITERAL(type) (type)

// --- logger customization macros --------------------------------------------

#define RKLOG_COLOR(R, G, B)\
    C_LITERAL(RKColor) {    \
        .r = (uint8_t)(R),  \
        .g = (uint8_t)(G),  \
        .b = (uint8_t)(B),  \
    }

#define RKLOG_CONFIG(TAG, BG_COLOR, FG_COLOR, USE_BG)\
    C_LITERAL(RKLogConfig) {                         \
        .tag = TAG,                                  \
        .background = BG_COLOR,                      \
        .foreground = FG_COLOR,                      \
        .useBackground = USE_BG,                     \
    }

#define RKLOG_CONFIG_BG(TAG, BG_COLOR, FG_COLOR)\
    C_LITERAL(RKLogConfig) {                    \
        .tag = TAG,                             \
        .background = BG_COLOR,                 \
        .foreground = FG_COLOR,                 \
        .useBackground = true,                  \
    }

#define RKLOG_CONFIG_NO_BG(TAG, FG_COLOR)\
    C_LITERAL(RKLogConfig) {             \
        .tag = TAG,                      \
        .background = RKLOG_COLOR_BLACK, \
        .foreground = FG_COLOR,          \
        .useBackground = false,          \
    }

#define RKLOG_STYLE(CFG_INFO, CFG_WARNING, CFG_ERROR, CFG_FATAL_ERROR)\
    C_LITERAL(RKLogStyle) {                                           \
        .cfgInfo = CFG_INFO,                                          \
        .cfgWarning = CFG_WARNING,                                    \
        .cfgError = CFG_ERROR,                                        \
        .cfgFatalError = CFG_FATAL_ERROR,                             \
    }

// --- logger customization defaults/presets ----------------------------------

#define RKLOG_COLOR_GREEN  RKLOG_COLOR(0, 255, 0)
#define RKLOG_COLOR_RED    RKLOG_COLOR(255, 0, 0)
#define RKLOG_COLOR_YELLOW RKLOG_COLOR(255, 255, 0)
#define RKLOG_COLOR_WHITE  RKLOG_COLOR(255, 255, 255)
#define RKLOG_COLOR_BLACK  RKLOG_COLOR(0, 0, 0)

#define RKLOG_DEFAULT_INFO_CFG\
    RKLOG_CONFIG_NO_BG("INFO", RKLOG_COLOR_GREEN)
#define RKLOG_DEFAULT_WARNING_CFG\
    RKLOG_CONFIG_NO_BG("WARNING",RKLOG_COLOR_YELLOW)
#define RKLOG_DEFAULT_ERROR_CFG\
    RKLOG_CONFIG_NO_BG("ERROR", RKLOG_COLOR_RED)
#define RKLOG_DEFAULT_FATAL_CFG\
    RKLOG_CONFIG_BG("FATAL", RKLOG_COLOR_RED, RKLOG_COLOR_WHITE)

#define RKLOG_DEFAULT_LOG_STYLE                  \
    C_LITERAL(RKLogStyle) {                      \
        .cfgInfo = RKLOG_DEFAULT_INFO_CFG,       \
        .cfgWarning = RKLOG_DEFAULT_WARNING_CFG, \
        .cfgError = RKLOG_DEFAULT_ERROR_CFG,     \
        .cfgFatalError = RKLOG_DEFAULT_FATAL_CFG,\
    }

// --- logger customization structs -------------------------------------------

/**
 * Struct representing a color used for foreground and background coloring of
 * log messages
 */
typedef struct
{
    uint8_t r; /* Red color channel */
    uint8_t g; /* Green color channel */
    uint8_t b; /* Blue color channel */
} RKColor;

/**
 * Struct containing log-type configuration settings
 */
typedef struct
{
    /* The tag title of the log message e.g. info, warning, error, fatal */
    const char* tag;
    /* Background color of the log message */
    RKColor background;
    /* Foreground color of the log message */
    RKColor foreground;
    /* Flag indicating whether the background color should be used */
    bool useBackground;
} RKLogConfig;

/**
 * Struct containing the styling for messages done by individual log severities
 */
typedef struct
{
    RKLogConfig cfgInfo;       /* Configuration for information logs */
    RKLogConfig cfgWarning;    /* Configuration for warning logs */
    RKLogConfig cfgError;      /* Configuration for error logs */
    RKLogConfig cfgFatalError; /* Configuration for fatal logs */
} RKLogStyle;

// --- logger interface -------------------------------------------------------

/**
 * Struct representing a handle to a logger
 */
typedef struct RKLogger RKLogger;

/**
 * @brief Creates a console logger with default presets
 *
 * @param[in] title
 *      The title of the console logger
 *
 * @return
 *      A pointer to the handle of the console logger, or `NULL` upon failure
 */
RKLogger* rkCreateDefaultLogger(const char* title);

/**
 * @brief Creates a file logger with default presets
 *
 * @param[in] fileName
 *      The name of the file to log to
 * @param[in] title
 *      The title of the file logger
 *
 * @return
 *      A pointer to the handle of the file logger, or `NULL` upon failure
 */
RKLogger* rkCreateDefaultFileLogger(const char* fileName, const char* title);

/**
 * @brief Creates a console logger with a custom style
 *
 * @param[in] title
 *      The title of the console logger 
 * @param[in] style
 *      The custom styling configuration for the console logger
 *
 * @return
 *      A pointer to the handle of the console logger, or `NULL` upon failure
 */
RKLogger* rkCreateLogger(const char* title, RKLogStyle style);

/**
 * @brief Creates a file logger with a custom style
 *
 * @param[in] fileName
 *      The name of the file to log to
 * @param[in] title
 *      The title of the file logger
 * @param[in] style
 *      The custom styling configuration for the file logger
 *
 * @return
 *      A pointer to the handle of the file logger, or `NULL` upon failure
 */
RKLogger* rkCreateFileLogger(const char* fileName, const char* title,
                             RKLogStyle style);

/**
 * @brief Frees all resources used by the logger. If `logger` is a file logger,
 * this will close the file before releasing the memory used by `logger`
 *
 * @param[in] logger
 *      A pointer to the handle of the logger to close
 */
void rkCloseLogger(RKLogger* logger);

/**
 * @brief Logs a formatted message using `logger` with the "info" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 */
void rkLogInfo(RKLogger* logger, const char* fmt, ...);

/**
 * @brief Logs a formatted message using `logger` with the "warning"
 * log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 */
void rkLogWarning(RKLogger* logger, const char* fmt, ...);

/**
 * @brief Logs a formatted message using `logger` with the "error" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 */
void rkLogError(RKLogger* logger, const char* fmt, ...);

/**
 * @brief Logs a formatted message using `logger` with the "fatal" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 */
void rkLogFatal(RKLogger* logger, const char* fmt, ...);

/**
 * @brief Logs a formatted message using a variadic argument list with the
 * "info" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 * @param[in] args
 *      The variadic arguments list 
 */
void rkLogInfoArgs(RKLogger* logger, const char* fmt, va_list args);

/**
 * @brief Logs a formatted message using a variadic argument list with the
 * "warning" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 * @param[in] args
 *      The variadic arguments list 
 */
void rkLogWarningArgs(RKLogger* logger, const char* fmt, va_list args);

/**
 * @brief Logs a formatted message using a variadic argument list with the
 * "error" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 * @param[in] args
 *      The variadic arguments list 
 */
void rkLogErrorArgs(RKLogger* logger, const char* fmt, va_list args);

/**
 * @brief Logs a formatted message using a variadic argument list with the
 * "fatal" log-severity
 *
 * @param[in] logger
 *      A pointer to the handle of the logger
 * @param[in] fmt
 *      The format specifier of the log message
 * @param[in] args
 *      The variadic arguments list 
 */
void rkLogFatalArgs(RKLogger* logger, const char* fmt, va_list args);

// --- implementation ---------------------------------------------------------

#if defined(RKLOG_IMPLEMENTATION)

#if defined(_WIN32)
#define RKLOG_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define RKLOG_PLATFORM_MACOS
#elif defined(__linux__)
#define RKLOG_PLATFORM_LINUX
#else
#error "Unsupported platform"
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#if defined(RKLOG_PLATFORM_WINDOWS)
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif

// --- string tokens ----------------------------------------------------------

#define RKLOG_TOKEN_ESCAPE_CODE_START "\033["
#define RKLOG_TOKEN_ESCAPE_CODE_END   "m"

#define RKLOG_TOKEN_ESCAPE_CODE_RESET\
    RKLOG_TOKEN_ESCAPE_CODE_START    \
    "0"                              \
    RKLOG_TOKEN_ESCAPE_CODE_END

#define RKLOG_TOKEN_TRUE_COLOR_MODE "2;"
#define RKLOG_TOKEN_256_COLOR_PALETTE_MODE "5;"

// --- formats ----------------------------------------------------------------

#define RKLOG_FMT_COLOR_PRELUDE  \
    RKLOG_TOKEN_ESCAPE_CODE_START\
    "38;2;%d;%d;%d;48;2;%d;%d;%d"\
    RKLOG_TOKEN_ESCAPE_CODE_END

#define RKLOG_FMT_COLOR_PRELUDE_NO_BG\
    RKLOG_TOKEN_ESCAPE_CODE_START    \
    "38;2;%d;%d;%d"                  \
    RKLOG_TOKEN_ESCAPE_CODE_END

#define RKLOG_FMT_LABEL "[%s]:[%s]:[%02d:%02d:%02d]: "

#define RKLOG_FMT_COLOR_OUTPUT "%s%s%s" RKLOG_TOKEN_ESCAPE_CODE_RESET "\n"

#define RKLOG_FMT_OUTPUT "%s%s\n"

// --- constants --------------------------------------------------------------

/**
 * Struct representing a specific point in time for when the log occured
 */
typedef struct
{
    uint32_t hours;   /* The system hours */
    uint32_t minutes; /* The system minutes */
    uint32_t seconds; /* The system seconds */
} RKTimeStamp;

/**
 * Struct definition for a logger
 */
struct RKLogger
{
#define RKLOG_MAX_LOGGER_TITLE_SIZE (64)
    /* The title of the logger */
    char title[RKLOG_MAX_LOGGER_TITLE_SIZE+1];
    /* The styling of the log messages of the logger */
    RKLogStyle style;
    /* The output stream where log messages gets logged to */
    FILE* output;
};

/**
 * @brief Allocates a new instance of a logger
 *
 * @param[in] out
 *      The output stream the logger should log to
 * @param[in] title
 *      The title of the logger
 * @param[in] style
 *      The styling configuration for the log messages
 *
 * @return
 *      A pointer to the newly allocated logger, or `NULL` upon failure. This
 *      can fail if `malloc` failed
 */
static RKLogger* rkNewLogger(FILE* out, const char* title, RKLogStyle style)
{
    RKLogger* const logger = (RKLogger*)malloc(sizeof(RKLogger));
    if (!logger) return NULL;

#if defined(RKLOG_PLATFORM_WINDOWS)
    const errno_t err = strcpy_s(
        logger->title,
        sizeof(char) * RKLOG_MAX_LOGGER_TITLE_SIZE,
        title
    );
    if (err != 0)
    {
        free(logger);
        return NULL;
    }
#else
    strncpy(logger->title, title, RKLOG_MAX_LOGGER_TITLE_SIZE);
#endif
    logger->style = style;
    logger->output = out;

#if defined(RKLOG_PLATFORM_WINDOWS)
    if (out == stderr)
    {
        const HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hErr, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hErr, dwMode);
    }
#endif

    return logger;
}

/**
 * @brief Generates the color specification prelude of the log message
 *
 * @param[in] buffer
 *      The buffer to which the color specification gets written to
 * @param[in] length
 *      The length of the buffer
 * @param[in] cfg
 *      The configuration for the specific log message to target
 */
static void rkGenColorPrelude(char* buffer, size_t length, RKLogConfig cfg)
{
    if (cfg.useBackground)
    {
        snprintf(buffer, length, RKLOG_FMT_COLOR_PRELUDE,
            cfg.foreground.r, cfg.foreground.g, cfg.foreground.b,
            cfg.background.r, cfg.background.g, cfg.background.b
        );
    }
    else
    {
        snprintf(buffer, length, RKLOG_FMT_COLOR_PRELUDE_NO_BG,
            cfg.foreground.r, cfg.foreground.g, cfg.foreground.b
        );
    }
}

/**
 * @brief Gets the current system time
 *
 * @return
 *      The current system time
 */
static RKTimeStamp rkGetCurrentTime(void)
{
    RKTimeStamp currTime = {0};
    
    struct tm* timeInfo = NULL;
    const time_t now = time(NULL);
    
#if defined(RKLOG_PLATFORM_WINDOWS)
    if (localtime_s(timeInfo, &now) != 0)
        return currTime;
#else
    timeInfo = localtime(&now);
#endif

    currTime.hours = (uint32_t)timeInfo->tm_hour;
    currTime.minutes = (uint32_t)timeInfo->tm_min;
    currTime.seconds = (uint32_t)timeInfo->tm_sec;

    return currTime;
}

/**
 * @brief Generates the label preceding the log message
 *
 * @param[in] buffer
 *      The buffer to which the label gets written to
 * @param[in] length
 *      The length of the buffer
 * @param[in] title
 *      The title of the logger
 * @param[in] tag
 *      The tag of the log message
 */
static void rkGenLabel(char* buffer, size_t length, const char* title,
                       const char* tag)
{
    const RKTimeStamp timeStamp = rkGetCurrentTime();   
    snprintf(
        buffer,
        length,
        RKLOG_FMT_LABEL,
        title,
        tag,
        timeStamp.hours,
        timeStamp.minutes,
        timeStamp.seconds
    );
}

/**
 * @brief Internal implementation of the logging operations. This logs a
 * formatted message to `out` using the provided variadic arguments list
 *
 * @param[in] out
 *      The output stream to log to
 * @param[in] title
 *      The title of the logger logging the message
 * @param[in] cfg
 *      The configuration of the log message
 * @param[in] fmt
 *      The format specifier of the log message
 * @param[in] args
 *      The variadic arguments list
 */
static void rkLogInternal(FILE* out, const char* title, RKLogConfig cfg,
                          const char* fmt, va_list args)
{
#define MAX_PRELUDE_SIZE (64)
#define MAX_LABEL_SIZE (64 + RKLOG_MAX_LOGGER_TITLE_SIZE)
#define MAX_MESSAGE_SIZE (256)

    char label[MAX_LABEL_SIZE+1] = {0};
    char message[MAX_MESSAGE_SIZE+1] = {0};
    
    rkGenLabel(label, MAX_LABEL_SIZE, title, cfg.tag);
    vsnprintf(message, MAX_MESSAGE_SIZE, fmt, args);
    
    if (out == stderr)
    {
        char prelude[MAX_PRELUDE_SIZE+1] = {0};
        rkGenColorPrelude(prelude, MAX_PRELUDE_SIZE, cfg);

        fprintf(out, RKLOG_FMT_COLOR_OUTPUT, prelude, label, message);
    }
    else
    {
        fprintf(out, RKLOG_FMT_OUTPUT, label, message);
    }
}

// --- rklog implementation ---------------------------------------------------

RKLogger* rkDefaultLogger(const char* title)
{
    return rkCreateLogger(title, RKLOG_DEFAULT_LOG_STYLE);
}

RKLogger* rkDefaultFileLogger(const char* fileName, const char* title)
{
    return rkCreateFileLogger(fileName, title, RKLOG_DEFAULT_LOG_STYLE);
}

RKLogger* rkCreateFileLogger(const char* fileName, const char* title,
                             RKLogStyle style)
{
#if defined(RKLOG_PLATFORM_WINDOWS)
    FILE* out = NULL;
    if (fopen_s(&out, fileName, "w") != 0)
        return NULL;
#else
    FILE* out = fopen(fileName, "w");
    if (!out) return NULL;
#endif

    return rkNewLogger(out, title, style);
}

RKLogger *rkCreateLogger(const char* title, RKLogStyle style)
{
    return rkNewLogger(stderr, title, style);
}

void rkCloseLogger(RKLogger* logger)
{
    if (logger->output != stderr)
        fclose(logger->output);

    free(logger);
}

void rkLogInfo(RKLogger* logger, const char* fmt, ...)
{
    va_list args = {0};

    va_start(args, fmt);
    rkLogInfoArgs(logger, fmt, args);
    va_end(args);
}

void rkLogWarning(RKLogger* logger, const char* fmt, ...)
{
    va_list args = {0};

    va_start(args, fmt);
    rkLogWarningArgs(logger, fmt, args);
    va_end(args);
}

void rkLogError(RKLogger* logger, const char* fmt, ...)
{
    va_list args = {0};

    va_start(args, fmt);
    rkLogErrorArgs(logger, fmt, args);
    va_end(args);
}

void rkLogFatal(RKLogger* logger, const char* fmt, ...)
{
    va_list args = {0};

    va_start(args, fmt);
    rkLogFatalArgs(logger, fmt, args);
    va_end(args);
}

void rkLogInfoArgs(RKLogger* logger, const char* fmt, va_list args)
{
    rkLogInternal(
        logger->output,
        logger->title,
        logger->style.cfgInfo,
        fmt,
        args
    );
}

void rkLogWarningArgs(RKLogger* logger, const char* fmt, va_list args)
{
    rkLogInternal(
        logger->output,
        logger->title,
        logger->style.cfgWarning,
        fmt,
        args
    );
}

void rkLogErrorArgs(RKLogger* logger, const char* fmt, va_list args)
{
    rkLogInternal(
        logger->output,
        logger->title,
        logger->style.cfgError,
        fmt,
        args
    );
}

void rkLogFatalArgs(RKLogger* logger, const char* fmt, va_list args)
{
    rkLogInternal(
        logger->output,
        logger->title,
        logger->style.cfgFatalError,
        fmt,
        args
    );
}

#endif /* RKLOG_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#endif /* __RKLOG_H__ */
