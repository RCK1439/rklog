#define RK_LOG_IMPLEMENTATION
#include "../rklog/rklog.h"

int main(void)
{
    // Let's define our own style for the logger!
    //
    // For RK_STYLE, the paremeters are the configurations for each level of
    // severity of the logs. i.e. info, warning, error and fatal
    //
    // For the use of RK_CONFIG, the paremeters are the tag, background color
    // and foreground color
    //
    // Then the easiest one is RK_COLOR where the paremeters are the R, G, B
    // values ranging from 0 to 255
    rkLogStyle style = RK_STYLE(
        RK_CONFIG("info", RK_COLOR(0, 128, 0), RK_COLOR(255, 255, 255)),
        RK_CONFIG("warning", RK_COLOR(128, 128, 0), RK_COLOR(255, 255, 255)),
        RK_CONFIG("error", RK_COLOR(128, 0, 0), RK_COLOR(255, 255, 255)),
        RK_CONFIG("critical", RK_COLOR(255, 0, 0), RK_COLOR(255, 255, 255))
    );

    // Then we can create our logger with the custom style
    rkLogger *logger = rkCreateLogger("custom", style);

    // Make some logs...
    rkLogInfo(logger, "custom info log!");
    rkLogWarning(logger, "custom warning log!");
    rkLogError(logger, "custom error log!");
    rkLogFatal(logger, "custom fatal log!");

    // Close the logger, obviously!
    rkCloseLogger(logger);
}

