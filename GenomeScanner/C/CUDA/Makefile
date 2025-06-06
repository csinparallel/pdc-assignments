## compile with both mpicc and nvcc to access mpi features in 
## the .c files and CUDA features in the .cu files
TARGET 	= genScanner
SRC    	= $(TARGET).c
LIB    	= helperFunctions
LIB_C  	= $(LIB).c
LIB_H  	= OO_MPI_IO.h $(LIB).h
CUDA   	= kernel
CUDA_C 	= $(CUDA).cu
OBJS   	= $(CUDA).o $(TARGET).o $(LIB).o

CC     	= mpicc
CUDA_CC	= nvcc
CFLAGS 	= -c -Wall -ansi -std=gnu11 -fopenmp # gnu used for strdup() support
LFLAGS 	= -o $(TARGET) -lm -lcudart -fopenmp
OS     	= $(shell uname -s)


ifeq ($(OS), Darwin)
CFLAGS += -Xclang -I/opt/homebrew/opt/libomp/include
LFLAGS += -L/opt/homebrew/opt/libomp/lib -lomp
else
CFLAGS += -pedantic -fopenmp
LFLAGS += -fopenmp
endif

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

$(CUDA).o: $(CUDA_C)
	$(CUDA_CC) $(CUDA_C) -c -x cu 

clean:
	rm -f $(TARGET) *.o *~ *#


