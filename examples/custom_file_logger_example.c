#define RK_LOG_IMPLEMENTATION
#include "../rklog/rklog.h"

int main(void)
{
    // First we can define our custom style!
    // 
    // NOTE: that we use RK_NO_BG_COL and RK_NO_FG_COL for the foreground and
    // background colors. In the context of file logging, this can be any
    // color you please since logging to files will ignore color.
    rkLogStyle style = RK_STYLE(
        RK_CONFIG("info", RK_NO_BG_COL, RK_NO_FG_COL),
        RK_CONFIG("warning", RK_NO_BG_COL, RK_NO_FG_COL),
        RK_CONFIG("error", RK_NO_BG_COL, RK_NO_FG_COL),
        RK_CONFIG("fatal", RK_NO_BG_COL, RK_NO_FG_COL)
    );

    // Next we create our file logger specifying the file to log to as well as
    // the title and style
    rkLogger *logger = rkCreateFileLogger("custom_file_logger_logs.txt", "custom_file_logger", style);

    // Make some logs to our file
    rkLogInfo(logger, "info log");
    rkLogWarning(logger, "warning log");
    rkLogError(logger, "error log");
    rkLogFatal(logger, "fatal log");

    // Close the logger
    rkCloseLogger(logger);
}