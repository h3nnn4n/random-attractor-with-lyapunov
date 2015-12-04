CC=gcc
FLAGS=$(CFLAGS) -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm -lpng -g

.PHONY: clean

all: clean quad

quad:
	$(CC) $(FLAGS) quad.c color.c      -o quad

spitter:
	$(CC) $(FLAGS) spitter.c color.c   -o spitter

animer:
	$(CC) $(FLAGS) color.c renderer.c  -o renderer
	$(CC) $(FLAGS) color.c animer.c    -o animer

clean:
	-@rm quad renderer animer spitter quad 2> /dev/null || true
