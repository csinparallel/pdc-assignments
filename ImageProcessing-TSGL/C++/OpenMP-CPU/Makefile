#Target to make and objects needed
TARGET = main
OBJS = $(TARGET).o ImageConverter.o

#Compiler, remove command, and OS we're working on
CC = g++
RM = rm -f
UNAME := $(shell uname)

#Check if we're on a Mac or a Linux machine

#Linux
ifeq ($(UNAME), Linux)
	OS_LFLAGS :=
	OS_INCLUDE :=
	OS_LDIRS := -L/usr/lib
	OS_EXTRA_LIB := 
	OS_GLFW := glfw
	OS_GL := -lGL
endif

#Mac
ifeq ($(UNAME), Darwin)
	OS_LFLAGS := -framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
	OS_INCLUDE := 
	OS_LDIRS := 
	OS_EXTRA_LIB := 
	OS_GLFW := glfw3
	OS_GL :=
	OS_EXTRA_LINK :=
endif

#Compiler flags
CXXFLAGS=-c -O3 -g3 \
    -Wall -Wextra -pedantic-errors \
    -I/usr/include/ \
    -I/usr/include/TSGL/ \
    ${OS_INCLUDE} \
    -I/usr/include/freetype2/ \
    -I/usr/include/freetype2/freetype  \
    -std=c++11 -fopenmp \
    -Wno-unused-function #Supress warnings

#Linking flags
LFLAGS=-o $(TARGET) \
    ${OS_EXTRA_LIB} \
    ${OS_LDIRS} \
    ${OS_EXTRA_LINK} \
    -ltsgl -lfreetype \
    -lGLEW -l${OS_GLFW} \
    -lX11 ${OS_GL} -lXrandr \
    -fopenmp \
    ${OS_LFLAGS} 

.SUFFIXES: .cpp .o

#all command
all: $(TARGET)

#Linking
$(TARGET): $(OBJS)
	@echo "\nLinking $(TARGET)..."
	$(CC) $(OBJS) $(LFLAGS)
	@echo
	
#Compiling
.cpp.o:
	@echo "\nCompiling $<..."
	$(CC) $(CXXFLAGS) $< 

#Indirect dependencies (Multiple file/classes only)
$(TARGET).o: ImageConverter.h
ImageConverter.o: ImageConverter.h

#Clean command
clean:
	$(RM) $(TARGET) $(OBJS) *~
