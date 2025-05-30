SHELL := /bin/bash
MODULE_LOAD = module load nvhpc/24.3

CC = nvc
CFLAGS = -fast -Minfo=accel -gpu=managed -acc=gpu
LFLAGS = -o
OBJ = circuitSolver

$(OBJ): $(OBJ).c
	$(SHELL) -c '$(MODULE_LOAD) && $(CC) $(CFLAGS) $(LFLAGS) $(OBJ) $(OBJ).c'

clean:
	rm -f $(OBJ)