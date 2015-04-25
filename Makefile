# cs335 lab1
# to compile your project, type make and press enter
CFLAGS = -I ./include
LIB    = 
LFLAGS = -std=c++0x -lX11 -lGLU -lGL -lm #-lXrandr

all: game.o hero.o Level.o gameObject.o platform.o main

mac:
	g++ main.cpp -Wall -oGame -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -std=c++11

mac2:
	g++ main.cpp game.cpp hero.cpp Level.cpp gameObject.cpp platform.cpp collisions.cpp -Wall -oGame -lX11 -lGL -lGLU -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -std=c++11

moy: ppm.cpp
	g++ main.cpp game.cpp hero.cpp Level.cpp gameObject.cpp platform.cpp collisions.cpp ppm.cpp -Wall -oGame -lX11 -lGL -lm -I/usr/X11R6/include -L/usr/X11R6/lib -lX11 -std=c++11

linux:
	g++ main.cpp -Wall -oGame -lX11 -lGL -lGLU -lm
main:  main.cpp
	g++ $(CFLAGS) main.cpp game.o hero.o Level.o gameObject.o platform.o -Wall -Wextra $(LFLAGS) -o Game
game.o:  game.h game.cpp
	g++ -c game.cpp
gameObject.o: gameObject.h gameObject.cpp
	g++ -c gameObject.cpp
hero.o:  hero.h hero.cpp
	g++ -c hero.cpp
Level.o: Level.h Level.cpp
	g++ -c Level.cpp
platform.o: platform.h platform.cpp
	g++ -c platform.cpp

Debug:
	g++ main.cpp -Wall -o Game -lX11 -lGL -lGLU -lm

clean:
	rm -f Game
	rm -f *.o

