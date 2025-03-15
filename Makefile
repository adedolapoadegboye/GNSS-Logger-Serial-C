# Compiler

CC = gcc

# Compiler flags

CFLAGS = -Wall -Wextra -pedantic -std=c11

# External libraries
EXTL = -lwiringPi

# Target executable

TARGET = basicgnsslogger

# Source files

SRCS = basicgnsslogger.c

# Object files

OBJS = $(SRCS:.c=.o)

# Default target

all: $(TARGET)

# Link object files to create the executable

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(EXTL)

# Compile source files into object files

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files

clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets

.PHONY: all clean


