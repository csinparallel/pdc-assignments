## compile with both mpicc and nvc to access mpi features in 
## the .c files and OpenACC features in the kernel files
TARGET = genScanner
SRC    = $(TARGET).c
LIB    = helperFunctions
LIB_C  = $(LIB).c
LIB_H  = OO_MPI_IO.h $(LIB).h
GPU	   = kernel
GPU_C  = $(GPU).c
OBJS   = $(TARGET).o $(LIB).o $(GPU).o

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

CC     = mpicc
GPU_CC = nvc
CFLAGS = -c -Wall -std=gnu99 # gnu used for strdup() support
# Uncomment -Minfo=accel for details about GPU offloading
ACC_CFLAGS = -c -acc=gpu #-Minfo=accel
LFLAGS = -o $(TARGET) -lm -lcudart -acc=gpu -lmpi_cxx -lmpi
OS     = $(shell uname -s)


ifeq ($(OS), Darwin)
CFLAGS += -Xclang -I/opt/homebrew/opt/libomp/include
LFLAGS += -L/opt/homebrew/opt/libomp/lib -lomp
else
CFLAGS += -pedantic -fopenmp
LFLAGS += -fopenmp
endif

$(TARGET): $(OBJS)
	$(MODULE_LOAD) && $(CC) $(OBJS) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

$(GPU).o: $(GPU_C)
	$(MODULE_LOAD) && $(GPU_CC) $(GPU_C) $(ACC_CFLAGS)


clean:
	rm -f $(TARGET) *.o *~ *#


