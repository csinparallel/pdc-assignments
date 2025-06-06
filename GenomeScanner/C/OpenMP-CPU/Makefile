TARGET = genScanner
SRC    = $(TARGET).c
LIB    = helperFunctions
LIB_C  = $(LIB).c
LIB_H  = OO_MPI_IO.h $(LIB).h
OBJS   = $(TARGET).o $(LIB).o

CC     = mpicc
CFLAGS = -c -Wall -ansi -std=gnu99 
LFLAGS = -o $(TARGET) 
OS     = $(shell uname -s)


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

clean:
	rm -f $(TARGET) *.o *~ *#


