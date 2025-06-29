# rklog

A simple header-only logging library used for explicit logging and debugging

## Usage

- Include the library in the file you wish to have logging. (You should define `RK_LOG_IMPLEMENTATION` in **only one** `*.c` file to allow for compilation of the library)
- Log as you please!

### Example

The following program;

```c
#define RK_LOG_IMPLEMENTATION
#include <rklog/rklog.h>

int main(void)
{
    rkLogInfo("Hello, World!");
    rkLogWarning("Oh no, a warning: %s", "there be dragons!");
    rkLogError("Some serious dragons!! Here's a number, why not: %d", 5);
    rkLogFatal("Cannot recover from this!");
}
```

yields the following output:

![image](resources/example.png)

### Basics

As seen in the example above, messages are logged in a format:
```
[severity]:[hh:mm:ss]: <message>
```

Based on the severity of the log, the color changes

## Future Plans

- Customizable logging colors
- Customizable logging formats
- Thread safe logging
- More portability