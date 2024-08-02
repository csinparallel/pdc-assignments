# Makefile for Linear Algebra Project
#  by Joel Adams, for CS 112 at Calvin University
#  Spring 2009.
# Modified for CSinParallel.org by 
# Steven McKelvey, Calvin University, Summer 2024.

# Variables
TARGET	= runTimeTrials
TESTS 	= tester

EXE		= $(TARGET) $(TESTS)

LIB_SRCS = MatrixTester.cpp MatrixTimer.cpp
CC=nvc++

# Uncomment -Minfo=mp for details about GPU offloading
# --diag_suppress included because g++ is not recognizing references within pragmas and gives warnings
CFLAGS=-fast -mp=gpu --diag_suppress=declared_but_not_referenced# -Minfo=mp

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

# Rules
.PHONY: all clean cleanLogs

all: $(TARGET) $(TESTS) 

# Compile source files
# For OpenMP GPU offloading, it appears that compilation needs to occur in one go
# (rather than compiling to object files and linking those files).
$(TARGET): $(TARGET).cpp MatrixTimer.cpp MatrixTimer.h Matrix.h
	@echo "Compiling $@..."
	@$(MODULE_LOAD) && $(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $< $(LIB_SRCS) \
	|| { echo "Compiliation failed for $@"; exit 1; }
	@echo "Finished compiling $@"
	@echo " "

$(TESTS): $(TESTS).cpp MatrixTester.cpp MatrixTester.h Matrix.h
	@echo "Compiling $@..."
	@$(MODULE_LOAD) && $(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $< $(LIB_SRCS) \
	|| { echo "Compiliation failed for $@"; exit 1; }
	@echo "Finished compiling $@"
	@echo " "
	
MatrixTester.h: Matrix.h
MatrixTimer.h: Matrix.h

clean:
	rm -f $(EXE) a.out *~ *# *.o

#cleans all log files
cleanLogs:
	rm -f trials*.txt
