#define RK_LOG_IMPLEMENTATION
#include "../rklog/rklog.h"

int main(void)
{
    // We create our file logger by specifying the file to log to and the
    // title of the logger
    rkLogger *logger = rkDefaultFileLogger("file_logger_logs.txt", "file_logger");

    // Then we can make some logs to that file
    rkLogInfo(logger, "info log");
    rkLogWarning(logger, "warning log");
    rkLogError(logger, "error log");
    rkLogFatal(logger, "fatal log");

    // And finally close the logger
    rkCloseLogger(logger);
}