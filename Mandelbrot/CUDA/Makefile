# Makefile for the Mandelbrot project
# To build mandel, enter: make
  
# Variables
TARGET      = mandel
SRC         = $(TARGET).cu
OBJ   		= $(TARGET).o

NVCC        = nvcc

CFLAGS      = -I/usr/include/TSGL \
			-I/usr/include/freetype2 \
			-I/usr/include/freetype2/freetype \
			-diag-suppress 20012 # suppress warnings related to the included files 
			
LFLAGS      = -lgomp -lm \
	  		-ltsgl -lfreetype -lGL -lGLEW -lglfw -lXrandr

# Rules
.PHONY: clean

# Link object files
$(TARGET): $(OBJ)
	@echo "Linking $@..."
	@$(NVCC) $^ $(LFLAGS) -o $@ \
	|| { echo "Linking failed for $@"; exit 1; }
	@echo "Finished building $@"
	@echo " "

$(OBJ): $(SRC)
	@echo "Compiling $< to $@..."
	@$(NVCC) $(CFLAGS) -c $< -o $@ \
	|| { echo "Compiliation failed for $<"; exit 1; }
	@echo "Finished compiling $<"
	@echo " "

clean:
	rm -f $(TARGET) *.o *~ *#

