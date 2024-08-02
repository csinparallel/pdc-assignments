# Makefile for OpenMP lab exercise
#  by Joel Adams, for CS 112 at Calvin University
#  Spring 2009.
# Adapted to compile CUDA code for CSinParallel.org

# Variables
TARGET	= runTimeTrials
TESTS 	= tester

EXE		= $(TARGET) $(TESTS)

NVCC=nvcc

CFLAGS=# -Wall
LFLAGS=-lgomp

# Rules
.PHONY: all clean

all: $(TARGET) $(TESTS)

# Link object files
$(TARGET): $(TARGET).o Matrix.o MatrixTimer.o
	@echo "Linking $@..."
	@$(NVCC) -o $@ $^ $(LFLAGS) \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

$(TESTS): $(TESTS).o MatrixTester.o Matrix.o 
	@echo "Linking $@..."
	@$(NVCC) -o $@ $^ $(LFLAGS) \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

# Compile source files to object files
%.o: %.cu
	@echo "Compiling $< to $@..."
	@$(NVCC) $(CFLAGS) -c $< -o $@ $(CFLAGS) \
	|| { echo "Compilation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "

# Dependencies
MatrixTester.o: MatrixTester.cuh Matrix.cuh
MatrixTimer.o: MatrixTimer.cuh Matrix.cuh
Matrix.o: Matrix.cuh
tester.o: MatrixTester.cuh
runTimeTrials.o: MatrixTimer.cuh Matrix.cuh

clean:
	rm -f $(EXE) a.out *~ *# *.o

#cleans all log files
cleanLogs:
	rm -f trials*.txt