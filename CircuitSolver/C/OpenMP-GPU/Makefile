SHELL := /bin/bash
MODULE_LOAD = module load nvhpc/24.3

OBJ = circuitSolver
CC = nvc
CFLAGS = -mp=gpu
LFLAGS = -o 

$(OBJ): $(OBJ).c
	$(SHELL) -c '$(MODULE_LOAD) && $(CC) $(CFLAGS) $(LFLAGS) $(OBJ) $(OBJ).c'

clean:
	rm -f $(OBJ)

