TARGET = testScanner
SRC    = $(TARGET).c
LIB    = helperFunctions
LIB_C  = ../$(LIB).c
LIB_H  = ../$(LIB).h
OBJS   = $(TARGET).o $(LIB).o

CC     = gcc
CFLAGS = -c -Wall -ansi -pedantic -std=c11
LFLAGS = -o $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LFLAGS)

$(TARGET).o: $(SRC) $(LIB_H)
	$(CC) $(SRC) $(CFLAGS)

$(LIB).o: $(LIB_C) $(LIB_H)
	$(CC) $(LIB_C) $(CFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#

