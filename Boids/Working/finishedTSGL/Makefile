all: tsglBoidsMP # tsglBoidsGPU


arg: GetArguments.cpp GetArguments.hpp
	g++ -c -o GetArguments.o GetArguments.cpp -Og


misc: misc.c
	nvc -c -fast misc.c -o misc.o


boidsMP: boids.cpp misc
	nvc++ -c -fast -fopenmp -mp -acc=multicore -Minfo=opt boids.cpp misc.o -o boidsMP.o


boidsGPU: boids.cpp misc
	nvc++ -c -fast -acc=gpu -gpu=managed -gpu=cc86 -Minfo=accel boids.cpp misc.o -o boidsGPU.o -DGPU


tsglBoidsMP: tsglBoids.cpp boidsMP misc arg
	nvc++ -fast tsglBoids.cpp boidsMP.o misc.o GetArguments.o -I$(TSGL_HOME)/include/TSGL -I$(TSGL_HOME)/include/freetype2 -ltsgl -lfreetype -lGLEW -lglfw -lGL -lGLU -o tsglBoidsMP -fopenmp -mp -acc=multicore -Minfo=opt


tsglBoidsGPU: tsglBoids.cpp boidsGPU misc arg
	nvc++ -fast tsglBoids.cpp boidsGPU.o misc.o -I$(TSGL_HOME)/include/TSGL -I$(TSGL_HOME)/include/freetype2 -ltsgl -lfreetype -lGLEW -lglfw -lGL -lGLU -o tsglBoidsGPU -acc=gpu -gpu=cc86 -Minfo=accel -DGPU


clean:
	rm -f *.o tsglBoidsGPU tsglBoidsMP