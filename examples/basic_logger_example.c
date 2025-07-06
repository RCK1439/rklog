#define RK_LOG_IMPLEMENTATION
#include "../rklog/rklog.h"

static const char *TITLE = "BASIC";

int main(void)
{
    // First we initialize our logger and give it a name!
    rkLogger *logger = rkDefaultLogger(TITLE);

    // Then, throughout our program, we can log using our logger
    rkLogInfo(logger, "Welcome to our basic example!");
    rkLogWarning(logger, "This one is yellow");
    rkLogError(logger, "This one is for some basic errors");
    rkLogFatal(logger, "This one is typically for scary (unrecoverable) errors");

    // We should remember to close our logger to prevent a memory leak!
    rkCloseLogger(logger);
}

