# Compiler
CC = gcc

# Compiler flags
CFLAGS =  -std=c99 -O2

# List of source files
SRCS = background.c bgprocess.c check.c hop.c log.c main.c proclore.c queue.c reveal.c seek.c

# List of header files (not strictly necessary in this simple case, but listed for clarity)
HDRS = background.h bgprocess.h hop.h log.h main.h proclore.h queue.h reveal.h seek.h

# Output executable
TARGET = a.out

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

# Compile each source file into an object file
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
