# Makefile for generateTestFiles.cpp

# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Werror

# Target executable
TARGET = generateTestFiles

# Source file
SRC = $(TARGET).cpp

# Object file
OBJ = $(TARGET).o

# Rules
.PHONY: all clean

all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build the object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $<

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET) a.out *~ *# *.o	