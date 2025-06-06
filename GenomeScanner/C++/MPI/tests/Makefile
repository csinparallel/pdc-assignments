TARGET = testScanner
SRC    = $(TARGET).cpp
LIB    = GenomeScanner
LIB_C  = ../$(LIB).cpp
LIB_H  = ../$(LIB).h
OBJS   = $(TARGET).o $(LIB).o

CC     = g++
CFLAGS = -c -Wall -ansi -pedantic -std=c++17
LFLAGS = -o $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#

