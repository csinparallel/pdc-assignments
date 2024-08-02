# Makefile for Linear Algebra Project
#  by Joel Adams, for CS 112 at Calvin University
#  Spring 2009.
# Adapted to compile C code for CSinParallel.org
# by Steven McKelvey, Calvin University, Summer 2024.

# Variables
TARGET	= runTimeTrials
TESTS 	= tester

EXE		= $(TARGET) $(TESTS)

CC		= gcc

CFLAGS	= -Wall -fopenmp
LFLAGS	= -fopenmp


# Rules
.PHONY: all clean cleanLogs

all: $(TARGET) $(TESTS) 

# Link object files
$(TARGET): $(TARGET).o Matrix.o MatrixTimer.o
	@echo "Linking $@..."
	@$(CC) $^ $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "
	
$(TESTS): $(TESTS).o MatrixTester.o Matrix.o 
	@echo "Linking $@..."
	@$(CC) $^ $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

# Compile source files to object files
%.o: %.c
	@echo "Compiling $< to $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "
	
# Dependencies
MatrixTester.o: MatrixTester.h Matrix.h
MatrixTimer.o: MatrixTimer.h Matrix.h
Matrix.o: Matrix.h
tester.o: MatrixTester.h
runTimeTrials.o: MatrixTimer.h Matrix.h

clean:
	rm -f $(EXE) a.out *~ *# *.o

#cleans all log files
cleanLogs:
	rm -f trials*.txt
