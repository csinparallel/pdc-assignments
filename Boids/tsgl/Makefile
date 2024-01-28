################################################################
omp: tsglBoidsOMP


mc: tsglBoidsMC


gpu: tsglBoidsGPU

all: omp mc gpu

################################################################

arg: GetArguments.cpp GetArguments.hpp
	g++ -c -o GetArguments.o GetArguments.cpp -O1


misc: misc.c
	gcc -c -O1 misc.c -o misc.o


boidsOMP: boids.cpp misc
	g++ -c -Ofast -fopenmp -Wall boids.cpp misc.o -o boidsOMP.o -DOMP


boidsMC: boids.cpp misc
	nvc++ -c -fast -fopenmp -mp -acc=multicore -Minfo=opt boids.cpp misc.o -o boidsMC.o -DMC


boidsGPU: boids.cpp misc
	nvc++ -c -fast -acc=gpu -gpu=managed -gpu=cc86 -Minfo=accel boids.cpp misc.o -o boidsGPU.o -DGPU


tsglBoidsOMP: tsglBoids.cpp boidsOMP misc arg
	g++ -Ofast tsglBoids.cpp boidsOMP.o misc.o GetArguments.o -I$(TSGL_HOME)/include/TSGL -I$(TSGL_HOME)/include/freetype2 -ltsgl -lfreetype -lGLEW -lglfw -lGL -lGLU -o tsglBoidsOMP -fopenmp -Wall -DOMP


tsglBoidsMC: tsglBoids.cpp boidsMC misc arg
	nvc++ -fast tsglBoids.cpp boidsMC.o misc.o GetArguments.o -I$(TSGL_HOME)/include/TSGL -I$(TSGL_HOME)/include/freetype2 -ltsgl -lfreetype -lGLEW -lglfw -lGL -lGLU -o tsglBoidsMC -fopenmp -mp -acc=multicore -Minfo=opt -DMC


tsglBoidsGPU: tsglBoids.cpp boidsGPU misc arg
	nvc++ -fast tsglBoids.cpp boidsGPU.o misc.o GetArguments.o -I$(TSGL_HOME)/include/TSGL -I$(TSGL_HOME)/include/freetype2 -ltsgl -lfreetype -lGLEW -lglfw -lGL -lGLU -o tsglBoidsGPU -acc=gpu -gpu=cc86 -Minfo=accel -DGPU


clean:
	rm -f *.o tsglBoidsGPU tsglBoidsMC tsglBoidsOMP