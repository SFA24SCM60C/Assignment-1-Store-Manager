# Makefile for Storage Manager Project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Output file
TARGET = test_assign1

# Source files
SRCS = dberror.c storage_mgr.c test_assign1_1.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Run tests
run: $(TARGET)
	./$(TARGET)
