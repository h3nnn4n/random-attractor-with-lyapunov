

all: spitter


quad:
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm quad.c color.c -lpng -o quad


spitter:
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm spitter.c color.c -lpng -o spitter

animer:
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm color.c renderer.c -lpng -o renderer
	gcc -Ofast -free -m64 -mtune=native -mfpmath=sse -msse4.2 -lm color.c animer.c -lpng -o animer
