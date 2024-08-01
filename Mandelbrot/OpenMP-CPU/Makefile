# Makefile for the Mandelbrot Project
# to build, enter: make

# Variables
PROG    = mandel
SRC     = $(PROG).cpp
OBJ     = $(PROG).o

CC      = g++
CFLAGS  = -Wall -ansi -pedantic -std=c++11 \
	  -I/usr/include/TSGL \
	  -I/usr/include/freetype2 \
	  -I/usr/include/freetype2/freetype \
	  -fopenmp
LFLAGS  = -lm \
	  -ltsgl -lfreetype -lGL -lGLEW -lglfw -lXrandr \
	  -fopenmp

# Rules
.PHONY: clean

# Link object files
$(PROG): $(OBJ)
	@echo "Linking $@..."
	@$(CC) $^ $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "
	
$(OBJ): $(SRC)
	@echo "Compiling $< to $@..."
	@$(CC) $(CFLAGS) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "

clean:
	rm -f $(PROG) *.o *~ *#
