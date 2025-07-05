# rklog

A simple header-only logging library used for explicit logging and debugging

## Usage

- Include the library in the file you wish to have logging. (You should define `RK_LOG_IMPLEMENTATION` in **only one** `*.c` file to allow for compilation of the library)
- Log as you please!

### Example

The following program;

```c
#define RK_LOG_IMPLEMENTATION
#include "rklog/rklog.h"

int main(void)
{
    rkLogger *const logger = rkCreateLogger(NULL, "YOUR_TITLE", RK_DEFAULT_LOG_STYLE);

    rkLogInfo(logger, "Hello, World!");
    rkLogWarning(logger, "Oh no, a warning: %s", "there be dragons!");
    rkLogError(logger, "Some serious dragons!! Here's a number, why not: %d", 5);
    rkLogFatal(logger, "Cannot recover from this!");

    rkCloseLogger(logger);
}

```

yields the following output:

![image](resources/example.png)

### Basics

As seen in the example above, messages are logged in a format:
```
[title]:[severity]:[hh:mm:ss]: <message>
```

In the example above `RK_DEFAULT_LOG_STYLE` was used for the logging style parameter. This can, of course be configured and styled in whichever way you please!

## Future Plans

- Customizable logging formats
- Thread safe logging (implemented, but untested)
- More portability