all: quad.c
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm quad.c color.c -lpng -o quad
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm color.c renderer.c -lpng -o renderer
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm color.c animer.c -lpng -o animer
