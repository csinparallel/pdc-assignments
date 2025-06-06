## compile with both mpic++ and nvc++ to access mpi features in 
## the .cpp files and OpenACC features in the kernel files
TARGET = genScanner
SRC    = $(TARGET).cpp
LIB    = GenomeScanner
LIB_C  = $(LIB).cpp
LIB_H  = $(LIB).h
GPU   = kernel
GPU_C = $(GPU).cpp
OBJS   = $(TARGET).o $(LIB).o $(GPU).o  

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

CC     = mpic++
GPU_CC = nvc++
# Add -fopenmp for OpenMP support
CFLAGS = -c -Wall -ansi -pedantic -std=c++17 -fopenmp
# Uncomment -Minfo=accel for details about GPU offloading
ACC_CFLAGS = -c -acc=gpu #-Minfo=accel
LFLAGS = -fopenmp -lcudart -acc=gpu -lmpi_cxx -lmpi

$(TARGET): $(OBJS)
	$(MODULE_LOAD) && $(CC) $(OBJS) -o $(TARGET) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

$(GPU).o: $(GPU_C)
	$(MODULE_LOAD) && $(GPU_CC) $(GPU_C) $(ACC_CFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#