CC = cc

CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c99

BASIC_EXAMPLE = basic_logger_example
CUSTOM_EXAMPLE = custom_logger_example

.PHONY: basic custom clean

basic:
	$(CC) $(CFLAGS) -o $(BASIC_EXAMPLE) basic_logger_example.c

custom:
	$(CC) $(CFLAGS) -o $(CUSTOM_EXAMPLE) custom_logger_example.c

clean:
	rm -f $(BASIC_EXAMPLE) $(CUSTOM_EXAMPLE)
