all: quad.c
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm quad.c -o quad
