all: tsglBoidsOMP


arg: GetArguments.cpp GetArguments.hpp
	g++ -c -o GetArguments.o GetArguments.cpp -Og -Wall


misc: misc.h misc.h
	gcc -c -o misc.o misc.c -Og -Wall


boidsMP: boids.cpp boids.hpp misc
	g++ -c -Ofast -fopenmp boids.cpp misc.o -o boidsMP.o -Wall


tsglBoidsOMP: tsglBoids.cpp boidsMP misc arg
	g++ -Ofast tsglBoids.cpp boidsMP.o misc.o GetArguments.o -I$(TSGL_HOME)/include/TSGL -I$(TSGL_HOME)/include/freetype2 -ltsgl -lfreetype -lGLEW -lglfw -lGL -lGLU -fopenmp -Wall -o tsglBoidsOMP


clean:
	rm -f *.o tsglBoidsOMP