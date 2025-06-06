## compile with both mpic++ and nvcc to access mpi features in 
## the .cpp files and CUDA features in the .cu files
TARGET = genScanner
SRC    = $(TARGET).cpp
LIB    = GenomeScanner
LIB_C  = $(LIB).cpp
LIB_H  = $(LIB).h
CUDA   = kernel
CUDA_C = $(CUDA).cu
OBJS   = $(CUDA).o $(TARGET).o $(LIB).o 

CC     = mpic++
CUDA_CC= nvcc
# Add -fopenmp for OpenMP support
CFLAGS = -c -Wall -ansi -pedantic -std=c++17 -fopenmp
LFLAGS = -o $(TARGET) -fopenmp -lcudart

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

$(CUDA).o: $(CUDA_C)
	$(CUDA_CC) $(CUDA_C) -c -x cu -std=c++17

clean:
	rm -f $(TARGET) *.o *~ *#