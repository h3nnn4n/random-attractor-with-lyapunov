CC=gcc
FLAGS=$(CFLAGS) -Ofast -free -march=native -mtune=native -mfpmath=sse -msse4.2 -lm -lpng -g

.PHONY: clean

all: clean quad

quad:
	$(CC) $(FLAGS) color.c quad.c      -o quad
	$(CC) $(FLAGS) color.c spitter.c   -o spitter
	$(CC) $(FLAGS) color.c renderer.c  -o renderer
	$(CC) $(FLAGS) color.c animer.c    -o animer

clean:
	-@rm quad renderer animer spitter quad 2> /dev/null || true
