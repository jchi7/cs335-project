# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -I ./include
LIB    = 
LFLAGS = $(LIB) -lX11 -lGLU -lGL -lm #-lXrandr

all: main

mac:
	g++ main.cpp -Wall -oGame -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11

linux:
	g++ main.cpp -Wall -oGame -lX11 -lGL -lGLU -lm
main:  main.cpp
	g++ $(CFLAGS) main.cpp -Wall -Wextra $(LFLAGS) -o Game

Debug:
	g++ main.cpp -Wall -o Game -lX11 -lGL -lGLU -lm

clean:
	rm -f Game
	rm -f *.o

