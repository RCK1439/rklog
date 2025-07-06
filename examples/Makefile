CC = cc

CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c99

BASIC_EXAMPLE = basic_logger_example
CUSTOM_EXAMPLE = custom_logger_example
FILE_EXAMPLE = file_logger_example
CUSTOM_FILE_EXAMPLE = custom_file_logger_example

.PHONY: all basic custom file custom_file clean

all: basic custom file custom_file

basic:
	$(CC) $(CFLAGS) -o $(BASIC_EXAMPLE) basic_logger_example.c

custom:
	$(CC) $(CFLAGS) -o $(CUSTOM_EXAMPLE) custom_logger_example.c

file:
	$(CC) $(CFLAGS) -o $(FILE_EXAMPLE) file_logger_example.c

custom_file:
	$(CC) $(CFLAGS) -o $(CUSTOM_FILE_EXAMPLE) custom_file_logger_example.c

clean:
	rm -f $(BASIC_EXAMPLE) $(CUSTOM_EXAMPLE) $(FILE_EXAMPLE) $(CUSTOM_FILE_EXAMPLE)
