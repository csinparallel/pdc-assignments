TARGET   = circuitSolverCpp11
SRC      = $(TARGET).cpp

CC       = g++
CFLAGS   = -Wall -ansi -pedantic -std=c++11
LFLAGS   = -o $(TARGET)

OS       = $(shell uname -s)
ifeq ($(OS), Linux)
LFLAGS   += -pthread
endif

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#

