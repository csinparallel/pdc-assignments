# Makefile for Linear Algebra Project
#  by Joel Adams, for CS 112 at Calvin University
#  Spring 2009.
# Adapted to compile C code for CSinParallel.org
# by Steven McKelvey, Calvin University, Summer 2024.

# Variables
TARGET	= runTimeTrials
TESTS 	= tester

EXE		= $(TARGET) $(TESTS)

LIBSRCS = MatrixTester.c MatrixTimer.c Matrix.c

CC=nvc

# Uncomment -Minfo=mp for details about GPU offloading
CFLAGS=-fast -mp=gpu -Minfo=mp

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

# Rules
.PHONY: all clean cleanLogs

all: $(TARGET) $(TESTS) 

$(TARGET): $(TARGET).c Matrix.c Matrix.h MatrixTimer.c MatrixTimer.h
	@echo  'Compiling: $<'
	$(MODULE_LOAD) && $(CC) $(CFLAGS) -o $@ $< $(LIBSRCS)
	@echo ' '

$(TESTS): $(TESTS).c Matrix.c Matrix.h MatrixTester.c MatrixTester.h 
	@echo  'Compiling: $<'
	$(MODULE_LOAD) && $(CC) $(CFLAGS) -o $@ $< $(LIBSRCS)
	@echo ' '
	
MatrixTester.h: Matrix.h
MatrixTimer.h: Matrix.h

clean:
	rm -f $(TARGET) $(TESTS) a.out *~ *# *.o

#cleans all log files
cleanLogs:
	rm -f trials*.txt