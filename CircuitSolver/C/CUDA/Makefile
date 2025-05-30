# Makefile for pinFinder project
# To build pinFinder, enter: make
# To build tests, enter: make tests

# Compiler
NVCC	= nvcc

TARGET	= circuitSolver
SRC	= $(TARGET).cu
OBJ_FILES	= $(TARGET).o


CFLAGS	= -Werror all-warnings -lgomp
LFLAGS	= -o $(TARGET) $(SRC) $(CFLAGS)


# Compilation: nvcc -o pinFinder pinFinder.cu helperFunctions.cu -lgomp
$(TARGET): $(SRC)
	$(NVCC) $(LFLAGS)



clean:
	rm -f $(TARGET) *.o *~ *#
