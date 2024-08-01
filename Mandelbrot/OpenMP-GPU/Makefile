# Makefile for the Mandelbrot Project
# to build, enter: make

# Variables
PROG    = mandel
SRC     = $(PROG).cpp
OBJ     = $(PROG).o

CC 		= nvc++

PATHS  = -I/usr/include/TSGL \
	  -I/usr/include/freetype2 \
	  -I/usr/include/freetype2/freetype

CFLAGS  = -fast -mp=gpu $(PATHS)# -Minfo=mp
	  
LFLAGS  = -lm \
	  -ltsgl -lfreetype -lGL -lGLEW -lglfw -lXrandr 

SHELL = /bin/bash
MODULE_LOAD = module load nvhpc-hpcx-cuda12/24.3

# Rules
.PHONY: clean

# Compile source files
# For OpenMP GPU offloading, it appears that compilation needs to occur in one go
# (rather than compiling to object files and linking those files).
$(PROG): $(SRC)
	@echo "Compiling $@..."
	@$(MODULE_LOAD) && $(CC) $(CFLAGS) $(LFLAGS) -o $@ $< \
	|| { echo "Compiliation failed for $@"; exit 1; }
	@echo "Finished compiling $@"
	@echo " "

clean:
	rm -f $(PROG) *.o *~ *#
