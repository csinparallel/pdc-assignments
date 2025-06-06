TARGET = genScanner
SRC    = $(TARGET).cpp
LIB    = GenomeScanner
LIB_C  = $(LIB).cpp
LIB_H  = OO_MPI_IO.h $(LIB).h
OBJS   = $(TARGET).o $(LIB).o

CC     = mpic++
CFLAGS = -c -Wall -ansi -pedantic -std=c++17 -c
LFLAGS = -o $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#

