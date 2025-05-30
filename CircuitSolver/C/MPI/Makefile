TARGET    = circuitSolver
SRC       = $(TARGET).c

CC      = mpicc
CFLAGS    = -Wall -ansi -pedantic -std=c11 
#CFLAGS    = -Wall -ansi -pedantic -std=c11 -O1 
#CFLAGS    = -Wall -ansi -pedantic -std=c11 -O2
#CFLAGS  = -Wall -ansi -pedantic -std=c11 -O3
LFLAGS  = -o $(TARGET) -lm

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LFLAGS)

clean:
	rm -f $(TARGET) *.o *~ *#
